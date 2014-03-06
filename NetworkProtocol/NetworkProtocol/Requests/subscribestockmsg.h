#ifndef SUBSCRIBESTOCKMSG_H
#define SUBSCRIBESTOCKMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
/*
 *  Request :: SubscribeStock
 *
 *  This message is for requesting subscription of stock with given id.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1><stock_id : 4>
 *
 *  where
 *      message_type         = MessageType::REQUEST_SUBSCRIBE_STOCK (0x1B)
 */
class NETWORKPROTOCOLSHARED_EXPORT SubscribeStock : public Request
{
    DTO::Types::StockIdType _stockId;

public:
    SubscribeStock(QDataStream&);

    DTO::Types::Message::MessageLengthType length() const;

    DTO::Types::Message::MessageType type() const;
    DTO::Types::StockIdType getStockId() const;

};
}
}
#endif // SUBSCRIBESTOCKMSG_H
