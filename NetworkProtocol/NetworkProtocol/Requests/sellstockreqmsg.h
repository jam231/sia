#ifndef SELLSTOCKREQMSG_H
#define SELLSTOCKREQMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
class NETWORKPROTOCOLSHARED_EXPORT SellStockReqMsg : public Request
{
    qint32 m_stockId;
    qint32 m_amount;
    qint32 m_price;


public:
    SellStockReqMsg(QDataStream& msg);

    MessageType type() const;

    qint16 length() const;

    qint32 getStockId() const;
    qint32 getAmount() const;
    qint32 getPrice() const;
};
}
}
#endif // SELLSTOCKREQMSG_H
