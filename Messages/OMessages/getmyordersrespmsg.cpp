#include "getmyordersrespmsg.h"

GetMyOrdersRespMsg::GetMyOrdersRespMsg()
{
}

qint16 GetMyOrdersRespMsg::length() const
{
    return sizeof(MessageType) + sizeof(qint32) + m_orders.size()*(sizeof(qint8) + 3*sizeof(qint32) + sizeof(qint32));
}

IOMessage::MessageType GetMyOrdersRespMsg::type() const
{
    return GET_MY_ORDERS_RESP;
}

void GetMyOrdersRespMsg::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);
    out << static_cast<qint32>(m_orders.size());
    for(int i = 0; i < m_orders.size(); ++i)
        out << static_cast<qint8>(m_orders[i].second.getTransactionType())
            << m_orders[i].first
            << m_orders[i].second.getStockId()
            << m_orders[i].second.getAmount()
            << m_orders[i].second.getPrice();

}

void GetMyOrdersRespMsg::addOrder(qint32 orderId, Order::OrderType m_transactionType, qint32 m_stockId,
                                  qint32 m_amount, qint32 m_price)
{
    m_orders.push_back(qMakePair(orderId, Order(m_transactionType, m_stockId, m_amount, m_price)));
}

void GetMyOrdersRespMsg::addOrder(qint32 orderId, Order order)
{
    m_orders.push_back(qMakePair(orderId, order));
}
