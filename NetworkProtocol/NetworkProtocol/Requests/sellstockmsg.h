#ifndef SELLSTOCKMSG_H
#define SELLSTOCKMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
/// TODO:   Add description with serialization format.
class NETWORKPROTOCOLSHARED_EXPORT SellStock : public Request
{
    DTO::Types::StockIdType _stockId;
    DTO::Types::AmountType _amount;
    DTO::Types::PriceType _price;


public:
    SellStock(QDataStream& msg);

    DTO::Types::StockIdType getStockId() const;
    DTO::Types::AmountType getAmount() const;
    DTO::Types::PriceType getPrice() const;

    DTO::Types::Message::MessageType type() const;
    DTO::Types::Message::MessageLengthType length() const;

};
}
}
#endif // SELLSTOCKMSG_H
