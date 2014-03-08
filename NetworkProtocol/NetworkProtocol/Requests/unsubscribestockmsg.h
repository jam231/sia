#ifndef UNSUBSCRIBESTOCKMSG_H
#define UNSUBSCRIBESTOCKMSG_H

#include "request.h"

#include "utilities.h"

#include <memory>

namespace NetworkProtocol {
namespace Requests
{
/*
 *  Request :: UnsubscribeStock
 *
 *  This message is for requesting end of subscription of stock with
 *  given stock id.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1><stock_id : 4>
 *
 *  where
 *      message_type         = MessageType::REQUEST_UNSUBSCRIBE_STOCK (0x1C)
 */
class NETWORKPROTOCOLSHARED_EXPORT UnsubscribeStock : public Request
{
    DTO::Types::StockIdType _stockId;

public:
    UnsubscribeStock(std::shared_ptr<AbstractLogger>, QDataStream&);
    UnsubscribeStock(QDataStream&);

    DTO::Types::Message::MessageLengthType length() const;

    DTO::Types::Message::MessageType type() const;
    DTO::Types::StockIdType getStockId() const;

};
}
}
#endif // UNSUBSCRIBESTOCKMSG_H
