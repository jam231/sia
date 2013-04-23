#ifndef MARKET_H
#define MARKET_H

#include "server.h"
#include "offer.h"

#include <QObject>
#include <QString>
#include <QSqlDatabase>

class Market : public QObject
{
    Q_OBJECT

    Server* m_server;
    QSqlDatabase m_database;

    static const int SERVER_PORT;
    static const QString DATABASE_USERNAME;
    static const QString DATABASE_PASSWORD;
    static const QString DATABASE_NAME;
    static const QString DATABASE_HOST;
    static const int DATABASE_PORT;

    void noticeChangePrice(qint32 stockId);
public:
    Market(QObject* parent = 0);

public slots:
    void registerUserReq(qint32 tmpId, double cash);
    void subscribeStock(qint32 userId, qint32 stockId);
    void unsubscribeStock(qint32 userId, qint32 stockId);
    void sellStock(qint32 userId, Offer offer);
    void buyStock(qint32 userId, Offer offer);
    void getStocks(qint32 userId);

};

#endif // MARKET_H
