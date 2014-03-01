#ifndef SELLTRANSACTIONMSG_H
#define SELLTRANSACTIONMSG_H

#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{
/*
 *  Responses :: SellTransaction
 *
 *  This message is for notyfing user about (complete) realization
 *  of one of his SELL type orders.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1><order_id : 4><amount : 4>
 *
 *  where
 *      message_type         = MessageType::RESPONSE_SELL_TRANSACTION
 */
class NETWORKPROTOCOLSHARED_EXPORT SellTransaction : public Response
{
    DTO::Types::OrderIdType _orderId;
    DTO::Types::AmountType _amount;

public:
    SellTransaction(DTO::Types::OrderIdType orderId, DTO::Types::AmountType amount);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

    void send(QIODevice *connection);
};

}
}
#endif // SELLTRANSACTIONMSG_H
