#ifndef SERWER_H
#define SERWER_H

#include "omessage.h"
#include "registeruserrespmsg.h"
#include "loginuserrespok.h"

#include "offer.h"
#include "connection.h"

#include <QObject>
#include <QTcpServer>

#include <map>
#include <exception>

class TcpConnectionError : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Tcp connection error.";
    }
};

using std::map;

class Server : public QObject
{    
    Q_OBJECT

    //static const int MAX_USER_PENDING;

    QTcpServer* m_server;
    map<qint32, Connection*> m_userConnections;
    int m_lastTmpUserId;
    //Connection* m_usersRegisterPending[500];

public:
    explicit Server(QObject *parent = 0, int portNumber = 6666);
    ~Server();
public slots:
    bool send(OMessage& msg, Connection*);
    bool send(RegisterUserRespMsg& msg, Connection*);
    bool send(LoginUserRespOk &msg, Connection*connection, qint32 userId);

signals:

    void registerUserRequestFromServer(Connection*, QString password);
    void loginUserRequestFromServer(Connection *connection,
                                    qint32 userId, QString password);
    void subscribeStock(qint32 userId, qint32 stockId);
    /*
    void unsubscribeStock(qint32 userId, qint32 stockId);
    void sellStock(qint32 userId, Offer offer);
    void buyStock(qint32 userId, Offer offer);
    void getStocks(qint32 userId);
*/


private slots:
    void addNewConnection();
    void disconnectUser(qint32 userId);
    void loginUser(Connection*, qint32, QString);
    //void assignUser(Connection*, qint32 userId, QString password);
    //void assignedUser(Connection* connection, qint32 userId);
    void registerUserRequest(Connection* connection, QString password);

};

#endif // SERWER_H
