#ifndef BUYSTOCKREQMSG_H
#define BUYSTOCKREQMSG_H

#include "imessage.h"
#include "offer.h"

#include <QIODevice>

class BuyStockReqMsg : public IMessage
{
    //Offer m_offer;
    qint32 m_stockId;
    qint32 m_amount;
    qint32 m_price;

    qint16 length() const;
public:
    BuyStockReqMsg(QDataStream&   msg);

    MessageType type() const;
    qint32 getStockId() const;
    qint32 getAmount() const;
    qint32 getPrice() const;
    //Offer offer() const;
};

#endif // BUYSTOCKREQMSG_H
