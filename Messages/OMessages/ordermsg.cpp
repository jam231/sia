#include "ordermsg.h"

#include <QDataStream>

OrderMsg::OrderMsg(Order::OrderType transactionType, qint32 stockId, qint32 amount, qint32 price)
    : m_order(transactionType, stockId, amount, price)
{   }

OrderMsg::OrderMsg(Order order) : m_order(order)
{   }

OrderMsg::OrderMsg() : m_order()
{   }

IOMessage::MessageType OrderMsg::type() const
{
    return ORDER;
}
void OrderMsg::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);
    out << m_order;
}
qint16 OrderMsg::length() const
{
    return sizeof(MessageType) + sizeof(qint8) + 3*sizeof(qint32);
}

qint32 OrderMsg::getStockId()
{
    return m_order.getStockId();
}
