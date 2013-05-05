#include "server.h"
#include "loginuserrespfail.h"

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

bool Server::send(OMessage& msg, Connection* connection)
{
    /*
    map<qint32, Connection*>::iterator it;
    it = m_userConnections.find(userId);
    if(it == m_userConnections.end())
        return false;
    it->second->send(msg);
    return true;
    */
    /* Zakładam, że weryfikacja tego, czy użytkownik został sprawdzony
     * już zaszła.
     */
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
    if(m_usersRegisterPending[userTmpId] == NULL)
        return false;
    m_usersRegisterPending[userTmpId]->setTmpUserId(-1);
    m_usersRegisterPending[userTmpId]->send(msg);o
    m_usersRegisterPending[userTmpId] = NULL;
    */

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
    map<qint32, Connection*>::iterator it;
    it = m_userConnections.find(userId);
    if(it == m_userConnections.end())
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
    Connection* newConn = new Connection(m_server->nextPendingConnection(),
                                         this);

    connect(newConn, SIGNAL(disconnected(qint32)),
              this, SLOT(disconnectUser(qint32)));
    //connect(newConn, SIGNAL(assigned(Connection*, qint32)),
    //          this, SLOT(assignedUser(Connection*, qint32)));

    connect(newConn, SIGNAL(registerUserRequestFromConnection(Connection*,
                                                              QString)),
              this, SLOT(registerUserRequest(Connection *, QString)));

    connect(newConn, SIGNAL(loginUserRequestFromConnection(Connection*,
                                                           qint32,QString)),
              this, SLOT(loginUser(Connection *, qint32, QString)));

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

void Server::loginUser(Connection* connection, qint32 userId,
                       QString password)
{
    emit loginUserRequestFromServer(connection, userId, password);
}

/*
void Server::assignUser(Connection* connection, qint32 userId,
                        QString password)
{

      map<qint32, Connection*>::iterator it;

      it = m_userConnections.find(userId);
      if(it == m_userConnections.end())
        it->second->deleteLater();
      m_userConnections[userId] = connection;

}
*/
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

