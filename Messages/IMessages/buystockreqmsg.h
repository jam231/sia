#ifndef BUYSTOCKREQMSG_H
#define BUYSTOCKREQMSG_H

#include "imessage.h"
#include "offer.h"

#include <QIODevice>

class BuyStockReqMsg : public IMessage
{
    Offer m_offer;
public:
    BuyStockReqMsg(QIODevice* msg);

    MessageType type() const;
    Offer offer() const;
};

#endif // BUYSTOCKREQMSG_H
