#include "transaction.h"

#include "user.h"
#include "stock.h"

#include <QSqlQuery>
#include <QString>
#include <QVariant>

Transaction::Transaction() : m_transactionId(-1), m_sellerId(-1), m_buyerId(-1),
    m_stockId(-1), m_numberStocks(-1), m_price(-1)
{
}

Transaction::Transaction(qint32 transactionId, qint32 sellerId, qint32 buyerId,
            qint32 stockId, qint32 numberStocks, double price)
    : m_transactionId(transactionId), m_sellerId(sellerId), m_buyerId(buyerId),
      m_stockId(stockId), m_numberStocks(numberStocks), m_price(price)
{}

Transaction::Transaction(qint32 transactionId, QSqlDatabase& db)
    : m_transactionId(transactionId)
{
    //zapytanie powinno zwrocic 1 rekord zwierajacy transakcje o danym id
    //rekord powinien zawierac id_sprzedawcy, id_kupujacego, id_akcji, ilosc akcji, cene
    QString queryString;

    QSqlQuery query(db);
    query.setForwardOnly(true);

    db.transaction();

    query.exec(queryString);

    db.commit();

    m_sellerId = query.value(0).toInt();
    m_buyerId = query.value(1).toInt();
    m_stockId = query.value(2).toInt();
    m_numberStocks = query.value(3).toInt();
    m_price = query.value(4).toDouble();
}

qint32 Transaction::transactionId() const
{
    return m_transactionId;
}
qint32 Transaction::sellerId() const
{
    return m_sellerId;
}
User Transaction::seller(QSqlDatabase& db) const
{
    return User(m_sellerId, db);
}
qint32 Transaction::buyerId() const
{
    return m_buyerId;
}
User Transaction::buyer(QSqlDatabase& db) const
{
    return User(m_buyerId, db);
}
qint32 Transaction::stockId() const
{
    return m_stockId;
}
Stock Transaction::stock(QSqlDatabase& db) const
{
    return Stock(m_stockId, db);
}
qint32 Transaction::numberStocks() const
{
    return m_numberStocks;
}
double Transaction::price() const
{
    return m_price;
}

QDataStream &operator<<(QDataStream& stream, const Transaction& transaction)
{
    stream<<transaction.m_transactionId<<transaction.m_sellerId<<transaction.m_buyerId
            <<transaction.m_stockId<<transaction.m_numberStocks<<transaction.m_price;

    return stream;
}
