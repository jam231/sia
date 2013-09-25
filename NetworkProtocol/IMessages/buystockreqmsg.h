#ifndef BUYSTOCKREQMSG_H
#define BUYSTOCKREQMSG_H

#include "imessage.h"

namespace NetworkProtocol {

class NETWORKPROTOCOLSHARED_EXPORT BuyStockReqMsg : public IMessage
{
    qint32 m_stockId;
    qint32 m_amount;
    qint32 m_price;

    qint16 length() const;
public:
    BuyStockReqMsg(QDataStream& msg);

    MessageType type() const;
    qint32 getStockId() const;
    qint32 getAmount() const;
    qint32 getPrice() const;
};

}
#endif // BUYSTOCKREQMSG_H
