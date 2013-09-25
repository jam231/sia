#include "order.h"

namespace NetworkProtocol {

Order::Order(Order::OrderType transactionType, qint32 stockId, qint32 amount, qint32 price)
    : m_transactionType(transactionType), m_stockId(stockId), m_amount(amount), m_price(price)
{
}

Order::Order() : m_transactionType(UNDEFINED), m_stockId(0), m_amount(0), m_price(0)
{
}

Order::OrderType Order::getTransactionType() const
{
    return m_transactionType;
}

qint32 Order::getStockId() const
{
    return m_stockId;
}

qint32 Order::getAmount() const
{
    return m_amount;
}

qint32 Order::getPrice() const
{
    return m_price;
}

Order::OrderType Order::toOrderType(int type)
{
    switch(type)
    {
    case BUY: return BUY;
    case SELL: return SELL;
    default: return UNDEFINED;
    };
}

QDataStream &operator<<(QDataStream& stream, const Order& order)
{
    stream << static_cast<qint8>(order.getTransactionType())
           << static_cast<qint32>(order.getStockId())
           << static_cast<qint32>(order.getAmount())
           << static_cast<qint32>(order.getPrice());

    return stream;
}

QDataStream &operator>>(QDataStream& stream, Order& order)
{
    qint8 transactionType;
    stream >> transactionType >> order.m_stockId >> order.m_amount >> order.m_price;
    order.m_transactionType = Order::toOrderType(transactionType);

    return stream;
}

}
