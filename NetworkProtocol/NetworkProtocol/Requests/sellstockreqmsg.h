#ifndef SELLSTOCKREQMSG_H
#define SELLSTOCKREQMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
class NETWORKPROTOCOLSHARED_EXPORT SellStockReqMsg : public Request
{
    DTO::Types::StockIdType m_stockId;
    DTO::Types::AmountType m_amount;
    DTO::Types::PriceType m_price;


public:
    SellStockReqMsg(QDataStream& msg);

    DTO::Types::StockIdType getStockId() const;
    DTO::Types::AmountType getAmount() const;
    DTO::Types::PriceType getPrice() const;

    DTO::Types::MessageType type() const;
    DTO::Types::MessageLengthType length() const;

    static const QString getMessageName();
};
}
}
#endif // SELLSTOCKREQMSG_H
