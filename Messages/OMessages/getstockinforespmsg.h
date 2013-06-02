#ifndef GETSTOCKINFORESPMSG_H
#define GETSTOCKINFORESPMSG_H

#include <QIODevice>
#include <omessage.h>
#include "order.h"

class GetStockInfoRespMsg : public OMessage
{
    Order m_bestBuyOrder;
    Order m_bestSellOrder;
    Order m_lastOrder;

    qint16 length() const;
public:
    GetStockInfoRespMsg(Order bestBuyOrder = Order(), Order bestSellOrder = Order(), Order lastOrder = Order());

    MessageType type() const;
    void send(QIODevice* connection);
};

#endif // GETSTOCKINFORESPMSG_H
