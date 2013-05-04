#include "server.h"

//const int Server::MAX_USER_PENDING = 500;

Server::Server(QObject *parent, int portNumber)
    : QObject(parent), m_server(new QTcpServer(this))//, m_lastTmpUserId(0)
{
    qDebug() << "[Server] Tworzenie połączenia na porcie"
             << portNumber << "...";
/*
    for(int i = 0; i < MAX_USER_PENDING; ++i)
        m_usersRegisterPending[i] = NULL;
*/
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

bool Server::send(OMessage& msg, qint32 userId)
{
    map<qint32, Connection*>::iterator it;
    it = m_userConnections.find(userId);
    if(it == m_userConnections.end())
        return false;
    it->second->send(msg);
    return true;
}
/*
 *  To wymaga jeszcze trocęe pracy.
 *  Wygląda to teraz tak, że w metodzie send robimy coś co moglibyśmy
 *  zrobić dużo wcześniej.
 *  Argumentem za takim zachowaniem jest seperacja warstwy komunikacji
 *  i warstwy związanej z bazą danych.
 *
 */
bool Server::send(RegisterUserRespMsg& msg, Connection* connection)
{
    /*
    if(m_usersRegisterPending[userTmpId] == NULL)
        return false;
    m_usersRegisterPending[userTmpId]->setTmpUserId(-1);
    m_usersRegisterPending[userTmpId]->send(msg);
    m_usersRegisterPending[userTmpId] = NULL;
    */
    return connection->send(msg);
}

void Server::addNewConnection()
{
    qDebug() << "[Server] Nawiązywanie nowego połączenia.";
    Connection* newConn = new Connection(m_server->nextPendingConnection(),
                                         this);

    connect(newConn, SIGNAL(disconnected(qint32)),
              this, SLOT(disconnectUser(qint32)));
    connect(newConn, SIGNAL(assigned(Connection*, qint32)),
              this, SLOT(assignedUser(Connection*, qint32)));

    connect(newConn, SIGNAL(registerUserRequestFromConnection(Connection*, QString)),
              this, SLOT(registerUserRequest(Connection *, QString)));

/* There is a duplicate name issue here which needs resolving.
 * There are already signals in Server class
 * (used in Market class) with those names.
 */
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
    newConn->start();
}

void Server::disconnectUser(qint32 userId)//, bool isTmpUser)
{
  /*
   *if(!isTmpUser)
   *      m_userConnections.erase(userId);
   * else m_usersRegisterPending[userId] = NULL;
   */
    qDebug() << "[Server] Usuwanie użytkownika " << userId
             << "z rejestru aktywnych użytkowników (lol).";
    if(m_userConnections.erase(userId) == 0)
    {
        qDebug() << "[Server] Próba usunięcia niepołączonego użytkownika.";
    }
}

void Server::assignedUser(Connection* connection, qint32 userId)
{
    map<qint32, Connection*>::iterator it;
    it = m_userConnections.find(userId);
    if(it == m_userConnections.end())
        it->second->deleteLater();

    m_userConnections[userId] = connection;
}

void Server::registerUserRequest(Connection* connection, QString password)
{/*
    for(int i = m_lastTmpUserId + 1; i < MAX_USER_PENDING; i = (i + 1) % MAX_USER_PENDING)
        if(m_usersRegisterPending[i] == NULL)
        {
            m_usersRegisterPending[i] = conn;
            m_usersRegisterPending[i]->setTmpUserId(i);
            m_lastTmpUserId = i;
            emit registerUserReq(i, password);
            return;
        }
        */
    emit registerUserRequestFromServer(connection, password);

}

