#include "server.h"

const int Server::MAX_USER_PENDING = 500;

Server::Server(QObject *parent, int portNumber)
    : QObject(parent), m_server(new QTcpServer(this)), m_lastTmpUserId(0)
{
    qDebug() << "[Server] Starting new tcp connection on port"
             << portNumber << "...";

    for(int i = 0; i < MAX_USER_PENDING; ++i)
        m_usersRegisterPending[i] = NULL;

    m_server = new QTcpServer(this);

    connect(m_server, SIGNAL(newConnection()),
            this, SLOT(addNewConnection()));

    if(!m_server->listen(QHostAddress::Any, portNumber))
    {
        qDebug() << "[Server] Error " << m_server->errorString()
                 << "occured while starting tcp connection.";
        throw TcpConnectionError();
    }

    qDebug() << "[Server] Tcp connection on port"
             << portNumber << " has been established.";

}

Server::~Server()
{
    m_server->close();
    qDebug() << "[Server] Tcp connection has been closed.";
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

bool Server::send(RegisterUserRespMsg& msg, qint32 userTmpId)
{
    if(m_usersRegisterPending[userTmpId] == NULL)
        return false;
    m_usersRegisterPending[userTmpId]->setTmpUserId(-1);
    m_usersRegisterPending[userTmpId]->send(msg);
    m_usersRegisterPending[userTmpId] = NULL;
    return true;
}

void Server::addNewConnection()
{
    qDebug() << "[Server] Acquiring new connection.";
    Connection* newConn = new Connection(m_server->nextPendingConnection(),
                                         this);

    connect(newConn, SIGNAL(disconnected(qint32, bool)),
              this, SLOT(disconnectUser(qint32, bool)));
    connect(newConn, SIGNAL(assigned(Connection *, qint32)),
              this, SLOT(assignedUser(Connection *, qint32)));
    connect(newConn, SIGNAL(registerReq(Connection *, QString)),
              this, SLOT(registerUserReq(Connection *, QString)));

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

void Server::disconnectUser(qint32 userId, bool isTmpUser)
{
    if(!isTmpUser)
         m_userConnections.erase(userId);
    else m_usersRegisterPending[userId] = NULL;
}

void Server::assignedUser(Connection* conn, qint32 userId)
{
    map<qint32, Connection*>::iterator it;
    it = m_userConnections.find(userId);
    if(it == m_userConnections.end())
        it->second->deleteLater();

    m_userConnections[userId] = conn;
}

void Server::registerUserReq(Connection* conn, QString password)
{
    for(int i = m_lastTmpUserId + 1; i < MAX_USER_PENDING; i = (i + 1) % MAX_USER_PENDING)
        if(m_usersRegisterPending[i] == NULL)
        {
            m_usersRegisterPending[i] = conn;
            m_usersRegisterPending[i]->setTmpUserId(i);
            m_lastTmpUserId = i;
            emit registerUserReq(i, password);
            return;
        }
}

