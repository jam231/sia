#ifndef OFFER_H
#define OFFER_H

class User;
class Stock;

#include <QSqlDatabase>
#include <QDataStream>

class Offer
{
    qint32 m_offerId;
    qint32 m_userId;
    qint32 m_stockId;
    qint32 m_numberStocks;
    double m_price;

public:
    Offer();
    Offer(qint32 offerId, qint32 userId, qint32 stockId, qint32 numberStocks, double price);
    Offer(qint32 userId, qint32 stockId, qint32 numberStocks, double price);
    Offer(qint32 offerId, QSqlDatabase& db);

    qint32 offerId() const;
    qint32 userId() const;
    User user(QSqlDatabase& db) const;
    qint32 stockId() const;
    Stock stock(QSqlDatabase& db) const;
    qint32 numberStocks() const;
    double price() const;

    friend QDataStream &operator<<(QDataStream& stream, const Offer& user);
    friend QDataStream &operator>>(QDataStream& stream, Offer& user);
};

#endif // OFFER_H
