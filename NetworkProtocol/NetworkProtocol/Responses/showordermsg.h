#ifndef SHOWORDERMSG_H
#define SHOWORDERMSG_H

#include "response.h"

#include "DataTransferObjects/order.h"

namespace NetworkProtocol
{
namespace Responses
{
/*
 *  Responses :: ShowOrder
 *
 *  This message is for sending user information about new order for stock with stockId.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1><order : 17>
 *
 *  where
 *      message_type    = MessageType::RESPONSE_SHOW_ORDER
 *      <order : 17>    = <order_id : 4><order_type : 1><stock_id : 4><amount : 4><price : 4>
 */
class NETWORKPROTOCOLSHARED_EXPORT ShowOrder : public Response
{
    DTO::Order _order;

public:
    ShowOrder(DTO::Types::OrderIdType, DTO::Types::Order::OrderType, DTO::Types::StockIdType,
          DTO::Types::AmountType, DTO::Types::PriceType);
    ShowOrder(DTO::Order order);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

    const DTO::Order& getOrder() const;

    void send(QIODevice *connection);

};

}
}
#endif // SHOWORDERMSG_H
