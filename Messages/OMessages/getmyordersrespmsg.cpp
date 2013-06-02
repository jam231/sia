#include "getmyordersrespmsg.h"

GetMyOrdersRespMsg::GetMyOrdersRespMsg()
{
}

qint16 GetMyOrdersRespMsg::length() const
{
    return sizeof(type()) + sizeof(qint32) + m_orders.size()*sizeof(Order);
}

IOMessage::MessageType GetMyOrdersRespMsg::type() const
{
    return GET_MY_ORDERS_RESP;
}

void GetMyOrdersRespMsg::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    out << length() << static_cast<qint8>(type())
        << static_cast<qint32>(m_orders.size());
    for(int i = 0; i < m_orders.size(); ++i)
        out << m_orders[i];
}

void GetMyOrdersRespMsg::addOrder(Order::OrderType m_transactionType, qint32 m_stockId, qint32 m_amount, qint32 m_price)
{
    m_orders.push_back(Order(m_transactionType, m_stockId, m_amount, m_price));
}

void GetMyOrdersRespMsg::addOrder(Order order)
{
    m_orders.push_back(order);
}
