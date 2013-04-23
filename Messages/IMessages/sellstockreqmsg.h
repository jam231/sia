#ifndef SELLSTOCKREQMSG_H
#define SELLSTOCKREQMSG_H

#include "imessage.h"
#include "offer.h"

#include <QIODevice>

class SellStockReqMsg : public IMessage
{
    Offer m_offer;
public:
    SellStockReqMsg(QIODevice* msg);

    MessageType type() const;
    Offer offer() const;
};

#endif // SELLSTOCKREQMSG_H
