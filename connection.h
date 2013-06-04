#ifndef CONNECTION_H
#define CONNECTION_H

#include "offer.h"
#include "omessage.h"

#include <QObject>
#include <QByteArray>
#include <QTcpSocket>
#include <ordermsg.h>

class Connection : public QObject
{
    Q_OBJECT
    static const qint32 NOT_ASSIGNED = -1;

    QByteArray m_buffer;

    QTcpSocket* m_socket;
    qint32 m_userId;
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
    bool send(OrderMsg& msg);
private slots:
    void disconect();
    void processIncomingMessages();
    bool processMessage();

signals:
    void disconnected(qint32 userId);

    void loginUserRequestFromConnection(Connection* connection,
                                        qint32, QString);
    void registerUserRequestFromConnection(Connection* connection,
                                           QString password);

    void sellStock(qint32 userId, qint32 stockId, qint32 amount, qint32 price);
    void buyStock(qint32 userId, qint32 stockId, qint32 amount, qint32 price);

    void getMyStocks(qint32 userId);
    void getMyOrders(qint32 userId);
    void getStockInfo(qint32 userId, qint32 stockId);

    void cancelOrder(qint32 userId, qint32 orderId);
};

#endif // CONNECTION_H
