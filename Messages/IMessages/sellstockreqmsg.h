#ifndef SELLSTOCKREQMSG_H
#define SELLSTOCKREQMSG_H

#include "imessage.h"
#include "offer.h"

#include <QDataStream>

class SellStockReqMsg : public IMessage
{
    qint32 m_stockId;
    qint32 m_amount;
    qint32 m_price;

    qint32 length() const;
public:
    SellStockReqMsg(QDataStream& msg);

    MessageType type() const;
    qint32 getStockId() const;
    qint32 getAmount() const;
    qint32 getPrice() const;
};

#endif // SELLSTOCKREQMSG_H
