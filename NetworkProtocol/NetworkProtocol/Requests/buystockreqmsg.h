#ifndef BUYSTOCKREQMSG_H
#define BUYSTOCKREQMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{

class NETWORKPROTOCOLSHARED_EXPORT BuyStockReqMsg : public Request
{
    DTO::Types::StockIdType m_stockId;
    DTO::Types::AmountType m_amount;
    DTO::Types::PriceType m_price;

public:
    BuyStockReqMsg(QDataStream& msg);

    DTO::Types::MessageType type() const;
    DTO::Types::MessageLengthType length() const;
    DTO::Types::StockIdType getStockId() const;
    DTO::Types::AmountType getAmount() const;
    DTO::Types::PriceType getPrice() const;
};
}
}
#endif // BUYSTOCKREQMSG_H
