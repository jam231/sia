#include "server.h"
#include "loginuserrespfail.h"


Server::Server(QObject *parent, int portNumber)
    : QObject(parent), m_server(new QTcpServer(this))
{
    qDebug() << "[Server] Tworzenie połączenia na porcie"
             << portNumber << "...";

    m_server = new QTcpServer(this);

    connect(m_server, SIGNAL(newConnection()),
            this, SLOT(addNewConnection()));

    if(!m_server->listen(QHostAddress::Any, portNumber))
    {
        qDebug() << "[Server] Wykryto błąd " << m_server->errorString()
                 << "podczas tworzenia połączenia tcp.";
        throw TcpConnectionError();
    }
    qDebug() << "[Server] Ustanowiono połączenie tcp na porcie"
             << portNumber << ".";
}

Server::~Server()
{
    m_server->close();
    qDebug() << "[Server] Połączenie tcp zostało zamknięte.";
    delete m_server;
}

/* TODO:
 *      Ta metoda jest dość nie fajna, bo własciwie potrzebna
 *      tylko do wysyłania RegisterUserFail oraz LoginUserFail.
 *
 *      Być może warto by było przeładować send dla tych dwóch metod
 *      i zostawić tylko send(OMessage& msg, qint32 userId)
 */
void Server::send(OMessage& msg, Connection* connection)
{

    connection->send(msg);
}
/*
 *  To wymaga jeszcze trochę pracy.
 *  Wygląda to teraz tak, że w metodzie send robimy coś co moglibyśmy
 *  zrobić dużo wcześniej.
 *  Argumentem za takim zachowaniem jest seperacja warstwy komunikacji
 *  i warstwy związanej z bazą danych.
 *                                          --jam231
 */
void Server::send(RegisterUserRespOk& msg, Connection* connection)
{
    // Jednak trzeba sie zalogowac świadomie ;-)
    //m_userConnections[msg.getUserId()] = connection;
    //connection->setUserId(msg.getUserId());

    connection->send(msg);
}

void Server::send(LoginUserRespOk& msg, Connection* connection,
                  qint32 userId)
{
    /*
     *  Sprawdź, czy jest już taki użytkownik w systemie.
     *  Jeżeli jest to wyślij wiadomość świadczącą o niepowodzeniu.
     */
    if(!m_userConnections.contains(userId))
    {
        m_userConnections[userId] = connection;
        connection->setUserId(userId);

        connection->send(msg);
    }
    else
    {
        qDebug() << "[Server] Próba zalogowania na aktywne konto.";
        LoginUserRespFail respMsg("Użytkownik już zalogowany.");

        connection->send(respMsg);
    }
}

void Server::send(OMessage& msg)
{
    for(auto it = m_userConnections.begin(); it != m_userConnections.end(); it++)
    {
        it.value()->send(msg);
    }
}

void Server::send(OMessage& msg, qint32 userId)
{
    /* TODO:
     *
     *      Wydajnie by bylo za jednym zamachem zwrocic wartosc value
     *      o ile m_userConnections zawiera klucz key
     */
    if(m_userConnections.contains(userId))
        m_userConnections[userId]->send(msg);
}







void Server::addNewConnection()
{
    qDebug() << "[Server] Nawiązywanie nowego połączenia.";
    Connection* newConnection = new Connection(m_server->nextPendingConnection(),
                                         this);

    connect(newConnection, SIGNAL(disconnected(qint32)),
              this, SLOT(disconnectUser(qint32)));
    connect(newConnection, SIGNAL(registerUserRequestFromConnection(Connection*,
                                                              QString)),
              this, SLOT(registerUserRequest(Connection *, QString)));

    connect(newConnection, SIGNAL(loginUserRequestFromConnection(Connection*,
                                                           qint32,QString)),
              this, SLOT(loginUserRequest(Connection *, qint32, QString)));


/*
    connect(newConn, SIGNAL(subscribeStock(qint32, qint32)),
            this, SLOT(subscribeStock(qint32, qint32)) );
    connect(newConn, SIGNAL(unsubscribeStock(qint32, qint32)),
            this, SLOT(unsubscribeStock(qint32, qint32)) );
*/
    connect(newConnection, SIGNAL(buyStock(qint32, qint32, qint32, qint32)),
            this, SIGNAL(buyStock(qint32, qint32, qint32, qint32)));
    connect(newConnection, SIGNAL(sellStock(qint32, qint32, qint32, qint32)),
            this, SIGNAL(sellStock(qint32, qint32, qint32, qint32)));

    connect(newConnection, SIGNAL(getMyStocks(qint32)),this, SIGNAL(getMyStocks(qint32)) );
    connect(newConnection, SIGNAL(getMyOrders(qint32)),this, SIGNAL(getMyOrders(qint32)) );

    //od teraz dopiero zaczniemy przetwarzac wiadomosci z tego polaczenia
    //abysmy nie robili tego zanim polaczylismy sygnaly i sloty
    //bo moglibysmy stracic wiadomosci
    newConnection->start();
}

void Server::disconnectUser(qint32 userId)
{
    qDebug() << "[Server] Usuwanie użytkownika " << userId
             << "z rejestru aktywnych użytkowników (lol).";
    // To się chyba nie powinno zdarzyć.
    if(m_userConnections.remove(userId) == 0)
    {
        qDebug() << "[Server] Próba usunięcia niepołączonego użytkownika.";
    }
}

void Server::loginUserRequest(Connection* connection, qint32 userId,
                       QString password)
{
    emit loginUserRequestFromServer(connection, userId, password);
}

void Server::registerUserRequest(Connection* connection, QString password)
{
    emit registerUserRequestFromServer(connection, password);

}
