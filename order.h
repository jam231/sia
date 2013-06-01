#ifndef ORDER_H
#define ORDER_H

#include <QtGlobal>
#include <QDataStream>

class Order
{
public:
    enum OrderType : qint8
    {
        UNDEFINED = 0,
        BUY = 1,
        SELL = 2
    };

private:
    OrderType m_transactionType;
    qint32 m_stockId;
    qint32 m_amount;
    qint32 m_price;
public:


    Order(OrderType, qint32, qint32, qint32);
    Order();

    OrderType getTransactionType() const;
    qint32 getStockId() const;
    qint32 getAmount() const;
    qint32 getPrice() const;

    static OrderType toOrderType(int type);

    friend QDataStream &operator<<(QDataStream& stream, const Order& order);
    friend QDataStream &operator>>(QDataStream& stream, Order& order);
};

#endif // ORDER_H
