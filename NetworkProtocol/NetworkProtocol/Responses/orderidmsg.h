#ifndef ORDERIDMSG_H
#define ORDERIDMSG_H

#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{
/*
 *  Responses :: OrderId
 *
 *  This message is for sending confirmation of acceptance of new order.
 *  Message contains order's id.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1><order_id : 4>
 *
 *  where
 *      message_type         = MessageType::RESPONSE_ORDER_ID;
 */
class NETWORKPROTOCOLSHARED_EXPORT OrderId : public Response
{
    DTO::Types::OrderIdType _orderId;

public:
    OrderId(DTO::Types::OrderIdType orderId);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;


    void send(QIODevice *connection);
};

}
}
#endif // ORDERIDMSG_H
