#ifndef SHOWNEWORDERMSG_H
#define SHOWNEWORDERMSG_H

#include "response.h"

#include "DataTransferObjects/order.h"

namespace NetworkProtocol
{
namespace Responses
{
/*
 *  Responses :: ShowNewOrder
 *
 *  This message is for sending user information about new order for stock with stock id.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1><order : 17>
 *
 *  where
 *      message_type    = MessageType::RESPONSE_SHOW_NEW_ORDER (0x19)
 *      <order : 17>    = <order_id : 4><order_type : 1><stock_id : 4><amount : 4><price : 4>
 */
class NETWORKPROTOCOLSHARED_EXPORT ShowNewOrder : public Response
{
    DTO::Order _order;

public:
    ShowNewOrder(DTO::Types::OrderIdType, DTO::Types::Order::OrderType,
                 DTO::Types::StockIdType, DTO::Types::AmountType,
                 DTO::Types::PriceType);
    ShowNewOrder(DTO::Order order);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

    const DTO::Order& getOrder() const;

    void send(QIODevice *connection);

};

}
}
#endif // SHOWNEWORDERMSG_H
