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
    static const qint32 NOT_ASSIGNED = -1;
    QTcpSocket* m_socket;
    qint32 m_userId;   //-1 oznacza "nie przypisany"
    qint32 m_tmpUserId; //Dla użytowników oczekujących na rejestracje
                        // dla pozostałych -1
public:
    Connection(QTcpSocket* socket, QObject *parent = 0);
    ~Connection();
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
    void disconnected(qint32 userId);
    void assigned(Connection* conn, qint32 userId);

    void registerUserRequestFromConnection(Connection* connection,
                                           QString password);

    void subscribeStock(qint32 userId, qint32 stockId);
    void unsubscribeStock(qint32 userId, qint32 stockId);
    void sellStock(qint32 userId, Offer offer);
    void buyStock(qint32 userId, Offer offer);
    void getStocks(qint32 userId);
};

#endif // CONNECTION_H
