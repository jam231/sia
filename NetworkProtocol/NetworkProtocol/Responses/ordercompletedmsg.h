#ifndef ORDERCOMPLETEDMSG_H
#define ORDERCOMPLETEDMSG_H

#include "response.h"


namespace NetworkProtocol
{
namespace Responses
{
/*
 *  Responses :: OrderCompleted
 *
 *  This message is for notyfing user about successful completion(/full realization)
 *  of one of his orders.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1><order_id : 4>
 *
 *  where
 *      message_type         = MessageType::RESPONSE_ORDER_COMPLETED
 */
class NETWORKPROTOCOLSHARED_EXPORT OrderCompleted : public Response
{
    DTO::Types::OrderIdType _orderId;

public:
    OrderCompleted(DTO::Types::OrderIdType orderId);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;


    void send(QIODevice *connection);
};

}
}
#endif // ORDERCOMPLETEDMSG_H
