#ifndef CONNECTION_H
#define CONNECTION_H

#include "offer.h"
#include "omessage.h"

#include <QObject>
#include <QSet>
#include <QByteArray>
#include <QTcpSocket>
#include <order.h>

class Connection : public QObject
{
    Q_OBJECT
    QByteArray m_buffer;
    static const qint32 NOT_ASSIGNED = -1;
    QTcpSocket* m_socket;
    qint32 m_userId;   //-1 oznacza "nie przypisany"
    QSet<qint32> m_subscribedStocks;

public:
    Connection(QTcpSocket* socket, QObject *parent = 0);
    ~Connection();
    int userId() const;
    bool isUserAssigned() const;
    //void setTmpUserId(qint32 tmpUserId);
    void setUserId(qint32);
    //dopiero od tego momentu zaczynamy odbierać wiadomości
    void start();
    void addSubscription(qint32 stockId);
    void dropSubscription(qint32 stockId);
public slots:
    bool send(OMessage& msg);
    bool send(Order& msg);
private slots:
    void disconect();
    void processIncomingMessages();
    void processMessage();

signals:
    void disconnected(qint32 userId);
    //void assigned(Connection* conn, qint32 userId);
    void loginUserRequestFromConnection(Connection* connection,
                                        qint32, QString);
    void registerUserRequestFromConnection(Connection* connection,
                                           QString password);

    //void subscribeStock(qint32 userId, qint32 stockId);
    //void unsubscribeStock(qint32 userId, qint32 stockId);
    void sellStock(qint32 userId, qint32 stockId, qint32 amount, qint32 price);
    void buyStock(qint32 userId, qint32 stockId, qint32 amount, qint32 price);
    //void getStocks(qint32 userId);
};

#endif // CONNECTION_H
