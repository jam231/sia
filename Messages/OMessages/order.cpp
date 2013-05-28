#include "order.h"

#include <QDataStream>

Order::Order(OrderType transactionType,
                               qint32 stockId, qint32 amount,
                               qint32 price)
{
    m_transactionType = transactionType;
    m_stockId = stockId;
    m_amount = amount;
    m_price = price;
}

Order::Order()
{
    m_transactionType = OrderType::UNDEFINED;
    m_stockId = 0;
    m_amount = 0;
    m_price = 0;
}

IOMessage::MessageType Order::type() const
{
    return ORDER;
}
void Order::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);
    out << static_cast<qint16>(2*sizeof(qint8) + sizeof(qint16) + 3*sizeof(qint32))
        << static_cast<qint8>(type())
        << static_cast<qint8>(m_transactionType)
        << static_cast<qint32>(m_stockId)
        << static_cast<qint32>(m_amount)
        << static_cast<qint32>(m_price);
}
qint32 Order::length() const
{
    return -1;
}

qint32 Order::getStockId()
{
    return m_stockId;
}
