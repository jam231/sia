#include "offer.h"

#include "user.h"
#include "stock.h"

#include <QString>
#include <QSqlQuery>
#include <QVariant>

Offer::Offer() : m_offerId(-1), m_userId(-1), m_stockId(-1),
    m_numberStocks(-1), m_price(-1)
{}

Offer::Offer(qint32 offerId, qint32 userId, qint32 stockId, qint32 numberStocks, double price)
    : m_offerId(offerId), m_userId(userId), m_stockId(stockId),
      m_numberStocks(numberStocks), m_price(price)
{
}

Offer::Offer(qint32 userId, qint32 stockId, qint32 numberStocks, double price)
    : m_offerId(-1), m_userId(userId), m_stockId(stockId),
      m_numberStocks(numberStocks), m_price(price)
{
}

Offer::Offer(qint32 offerId, QSqlDatabase& db) : m_offerId(offerId)
{
    //zapytanie powinno zwrocic 1 rekord zwierajacy oferte o danym id
    //rekord powinien zawierac id_sprzedawcy/kupujacego, id_akcji, ilosc akcji, cene
    QString queryString;

    QSqlQuery query(db);
    query.setForwardOnly(true);

    db.transaction();

    query.exec(queryString);

    db.commit();

    m_userId = query.value(0).toInt();
    m_stockId = query.value(1).toInt();
    m_numberStocks = query.value(2).toInt();
    m_price = query.value(3).toDouble();
}

qint32 Offer::offerId() const
{
    return m_offerId;
}
qint32 Offer::userId() const
{
    return m_userId;
}
User Offer::user(QSqlDatabase& db) const
{
    return User(m_userId, db);
}
qint32 Offer::stockId() const
{
    return m_stockId;
}
Stock Offer::stock(QSqlDatabase& db) const
{
    return Stock(m_stockId, db);
}
qint32 Offer::numberStocks() const
{
    return m_numberStocks;
}
double Offer::price() const
{
    return m_price;
}

QDataStream &operator<<(QDataStream& stream, const Offer& offer)
{
    stream<<offer.m_offerId<<offer.m_userId<<offer.m_stockId<<offer.m_numberStocks<<offer.m_price;
    return stream;
}

QDataStream &operator>>(QDataStream& stream, Offer& offer)
{
    offer.m_offerId = -1;
    stream>>offer.m_userId>>offer.m_stockId>>offer.m_numberStocks>>offer.m_price;
    return stream;
}
