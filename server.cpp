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

bool Server::send(OMessage& msg, Connection* connection)
{

    return connection->send(msg);
}
/*
 *  To wymaga jeszcze trochę pracy.
 *  Wygląda to teraz tak, że w metodzie send robimy coś co moglibyśmy
 *  zrobić dużo wcześniej.
 *  Argumentem za takim zachowaniem jest seperacja warstwy komunikacji
 *  i warstwy związanej z bazą danych.
 *                                          --jam231
 */
bool Server::send(RegisterUserRespMsg& msg, Connection* connection)
{

    /*
     *  Przez tę metodę przechodzi odpowiedź z już wygenerowanym id.
     *  To odpowiednia chwila na dodanie do "rejestru" aktywnych
     *  użytkowników.
     *  Nawet, gdy połączenie się zerwie podczas dodawnia nowego użytkownika
     *  to sygnał disconnect() docierajacy do slotu Server::disconnectUser()
     *  spowoduje oczyszczenie rejestru.
     *
     *  Pewien dyskomfort, może budzić sytuacja w której połączenie
     *  zostanie zerwane przed wywyłaniem connection->send(msg).
     *  Okazuje się (po moich licznych testach), że w takiej sytuacji
     *  connection nie jest odrazu usuwane (pewnie dzieki deleteLater())
     *  wszystko sie poprawnie wywoluje, jedynie dane zapisane
     *  w zamknietym gniezdzie sa odrzucane. Nie mniej nalezaloby
     *  upewnić się co do poprawności tego konstruktu.
     */
    m_userConnections[msg.getUserId()] = connection;
    connection->setUserId(msg.getUserId());

    return connection->send(msg);
}

bool Server::send(LoginUserRespOk& msg, Connection* connection,
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

        return connection->send(msg);
    }
    else
    {
        qDebug() << "[Server] Próba zalogowania na aktywne konto.";
        LoginUserRespFail respMsg("Użytkownik już zalogowany.");

        return connection->send(respMsg);
    }
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
              this, SLOT(loginUser(Connection *, qint32, QString)));


/*
    connect(newConn, SIGNAL(subscribeStock(qint32, qint32)),
            this, SLOT(subscribeStock(qint32, qint32)) );
    connect(newConn, SIGNAL(unsubscribeStock(qint32, qint32)),
            this, SLOT(unsubscribeStock(qint32, qint32)) );
    connect(newConn, SIGNAL(sellStock(qint32, Offer)),
            this, SLOT(sellStock(qint32, Offer)) );
    connect(newConn, SIGNAL(buyStock(qint32, Offer)),
            this, SLOT(buyStock(qint32, Offer)) );
    connect(newConn, SIGNAL(getStocks(qint32)),this, SLOT(getStocks(qint32)) );
*/
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

void Server::loginUser(Connection* connection, qint32 userId,
                       QString password)
{
    emit loginUserRequestFromServer(connection, userId, password);
}


void Server::registerUserRequest(Connection* connection, QString password)
{
    emit registerUserRequestFromServer(connection, password);

}

