#ifndef BUYSTOCKREQMSG_H
#define BUYSTOCKREQMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{

class NETWORKPROTOCOLSHARED_EXPORT BuyStock : public Request
{
    DTO::Types::StockIdType m_stockId;
    DTO::Types::AmountType m_amount;
    DTO::Types::PriceType m_price;

public:
    BuyStock(QDataStream& msg);

    DTO::Types::Message::MessageType type() const;
    DTO::Types::MessageLengthType length() const;
    DTO::Types::StockIdType getStockId() const;
    DTO::Types::AmountType getAmount() const;
    DTO::Types::PriceType getPrice() const;

    static const QString getMessageName();
};
}
}
#endif // BUYSTOCKREQMSG_H
