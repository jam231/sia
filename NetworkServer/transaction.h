#ifndef TRANSACTION_H
#define TRANSACTION_H

class User;
class Stock;

#include <QDataStream>
#include <QSqlDatabase>

class Transaction
{
    qint32 m_transactionId;

    qint32 m_sellerId;
    qint32 m_buyerId;

    qint32 m_stockId;

    qint32 m_numberStocks;
    double m_price;

public:
    Transaction();
    Transaction(qint32 transactionId, qint32 sellerId, qint32 buyerId,
                qint32 stockId, qint32 numberStocks, double price);
    Transaction(qint32 transactionId, QSqlDatabase& db);

    qint32 transactionId() const;
    qint32 sellerId() const;
    User seller(QSqlDatabase& db) const;
    qint32 buyerId() const;
    User buyer(QSqlDatabase& db) const;
    qint32 stockId() const;
    Stock stock(QSqlDatabase& db) const;
    qint32 numberStocks() const;
    double price() const;

    friend QDataStream &operator<<(QDataStream& stream, const Transaction& eransaction);
};

#endif // TRANSACTION_H
