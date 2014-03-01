#ifndef BUYTRANSACTIONMSG_H
#define BUYTRANSACTIONMSG_H

#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{
/*
 *  Responses :: BuyTransaction
 *
 *  This message is for notyfing user about (complete) realization
 *  of one of his BUY type orders.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1><order_id : 4><amount : 4>
 *
 *  where
 *      message_type         = MessageType::RESPONSE_BUY_TRANSACTION
 */
class NETWORKPROTOCOLSHARED_EXPORT BuyTransaction : public Response
{
    DTO::Types::OrderIdType _orderId;
    DTO::Types::AmountType _amount;

public:
    BuyTransaction(DTO::Types::OrderIdType orderId, DTO::Types::AmountType amount);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;


    void send(QIODevice *connection);
};

}
}
#endif // BUYTRANSACTIONMSG_H
