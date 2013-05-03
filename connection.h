#ifndef CONNECTION_H
#define CONNECTION_H

#include "offer.h"
#include "omessage.h"

#include <QObject>
#include <QByteArray>
#include <QTcpSocket>

class Connection : public QObject
{
    Q_OBJECT
    QByteArray buffer;

    QTcpSocket* m_socket;
    qint32 m_userId;   //-1 = not assigned
    qint32 m_tmpUserId; //for user pending to register, for others -1
public:
    Connection(QTcpSocket* socket, QObject *parent = 0);

    int userId() const;
    bool isUserAssigned() const;
    void setTmpUserId(qint32 tmpUserId);
    //dopiero od tego momentu zaczynamy odbierać wiadomości
    void start();

public slots:
    bool send(OMessage& msg);

private slots:
    void disconect();
    void readData();

signals:
    void disconnected(qint32 userId, bool isTmpUserId);
    void assigned(Connection* conn, qint32 userId);

    void registerReq(Connection* conn, QString password);

    void subscribeStock(qint32 userId, qint32 stockId);
    void unsubscribeStock(qint32 userId, qint32 stockId);
    void sellStock(qint32 userId, Offer offer);
    void buyStock(qint32 userId, Offer offer);
    void getStocks(qint32 userId);
};

#endif // CONNECTION_H
