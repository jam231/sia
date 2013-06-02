#ifndef GETMYORDERSRESPMSG_H
#define GETMYORDERSRESPMSG_H

#include <order.h>
#include <omessage.h>
#include <QVector>


class GetMyOrdersRespMsg : public OMessage
{
    QVector<Order> m_orders;

    qint16 length() const;
public:
    GetMyOrdersRespMsg();

    MessageType type() const;
    void send(QIODevice* connection);

    void addOrder(Order::OrderType m_transactionType, qint32 m_stockId, qint32 m_amount,
                  qint32 m_price);
    void addOrder(Order order);
};

#endif // GETMYORDERSRESPMSG_H
