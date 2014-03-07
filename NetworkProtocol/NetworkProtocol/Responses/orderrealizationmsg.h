#ifndef ORDERREALIZATIONMSG_H
#define ORDERREALIZATIONMSG_H

#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{
/*
 *  Responses :: OrderRealization
 *
 *  This message is for notyfing user about (semi or full) realization
 *  of one of his orders.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1><order_id : 4><amount : 4>
 *
 *  where
 *      message_type         = MessageType::RESPONSE_ORDER_REALIZATION
 */
class NETWORKPROTOCOLSHARED_EXPORT OrderRealization : public Response
{
    DTO::Types::OrderIdType _orderId;
    DTO::Types::AmountType _amount;

public:
    OrderRealization(DTO::Types::OrderIdType orderId, DTO::Types::AmountType amount);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;


    void send(QIODevice *connection);
};

}
}
#endif // ORDERREALIZATIONMSG_H
