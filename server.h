#ifndef SERWER_H
#define SERWER_H

#include "omessage.h"
#include "registeruserrespmsg.h"

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

    static const int MAX_USER_PENDING;

    QTcpServer* m_server;
    map<qint32, Connection*> m_userConnections;
    int m_lastTmpUserId;
    Connection* m_usersRegisterPending[500];

public:
    explicit Server(QObject *parent = 0, int portNumber = 6666);
    ~Server();
public slots:
    bool send(OMessage& msg, qint32 userId);
    bool send(RegisterUserRespMsg& msg, qint32 userTmpId);

signals:

    void registerUserReq(qint32 tmpUserId, QString password);
    void subscribeStock(qint32 userId, qint32 stockId);
    void unsubscribeStock(qint32 userId, qint32 stockId);
    void sellStock(qint32 userId, Offer offer);
    void buyStock(qint32 userId, Offer offer);
    void getStocks(qint32 userId);



private slots:
    void addNewConnection();
    void disconnectUser(qint32 userId, bool isTmpUser);
    void assignedUser(Connection* conn, qint32 userId);
    void registerUserReq(Connection* conn, QString password);

};

#endif // SERWER_H
