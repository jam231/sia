#ifndef ORDERREALIZATIONMSG_H
#define ORDERREALIZATIONMSG_H

#include "response.h"

#include "utilities.h"

#include <memory>

namespace NetworkProtocol
{
namespace Responses
{
/*
 *  Responses :: OrderRealization
 *
 *  This message is for notyfing user about (partial or full) realization
 *  of one of his orders.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1><order_id : 4><amount : 4><price : 4>
 *
 *  where
 *      message_type         = MessageType::RESPONSE_ORDER_REALIZATION (0x17)
 */
class NETWORKPROTOCOLSHARED_EXPORT OrderRealization : public Response
{
    DTO::Types::OrderIdType _orderId;
    DTO::Types::AmountType _amount;
    DTO::Types::PriceType _price;

public:

    OrderRealization(std::shared_ptr<AbstractLogger>,
                     DTO::Types::OrderIdType, DTO::Types::AmountType,
                     DTO::Types::PriceType);
    OrderRealization(DTO::Types::OrderIdType, DTO::Types::AmountType,
                     DTO::Types::PriceType);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;


    void send(QIODevice *connection);
};

}
}
#endif // ORDERREALIZATIONMSG_H
