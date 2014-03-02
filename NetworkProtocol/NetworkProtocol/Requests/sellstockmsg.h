#ifndef SELLSTOCKMSG_H
#define SELLSTOCKMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
/*
 *  Request :: SellStock
 *
 *  This message is for requesting SELL order for stock with given id.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1><stock_id : 4><amount : 4><price : 4>
 *
 *  where
 *      message_type         = MessageType::REQUEST_SELL_STOCK_ORDER
 */
class NETWORKPROTOCOLSHARED_EXPORT SellStock : public Request
{
    DTO::Types::StockIdType _stockId;
    DTO::Types::AmountType _amount;
    DTO::Types::PriceType _price;


public:
    SellStock(QDataStream&);

    DTO::Types::StockIdType getStockId() const;
    DTO::Types::AmountType getAmount() const;
    DTO::Types::PriceType getPrice() const;

    DTO::Types::Message::MessageType type() const;
    DTO::Types::Message::MessageLengthType length() const;

};
}
}
#endif // SELLSTOCKMSG_H
