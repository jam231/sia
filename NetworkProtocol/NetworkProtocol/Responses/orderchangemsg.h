#ifndef OrderChangeMSG_H
#define OrderChangeMSG_H

#include "response.h"

#include "utilities.h"

#include <memory>

namespace NetworkProtocol
{
namespace Responses
{
/*
 *  Responses :: OrderChange
 *
 *  This message is for notyfing user about (partial or full) realization
 *  of one of his orders.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1><order_id : 4><amount : 4><price : 4>
 *
 *  where
 *      message_type         = MessageType::RESPONSE_ORDER_CHANGE (0x17)
 */
class NETWORKPROTOCOLSHARED_EXPORT OrderChange : public Response
{
    DTO::Types::OrderIdType _orderId;
    DTO::Types::AmountType _amount;
    DTO::Types::PriceType _price;

public:

    OrderChange(std::shared_ptr<AbstractLogger>,
                     DTO::Types::OrderIdType, DTO::Types::AmountType,
                     DTO::Types::PriceType);
    OrderChange(DTO::Types::OrderIdType, DTO::Types::AmountType,
                     DTO::Types::PriceType);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;


    void send(QIODevice *connection);

    DTO::Types::OrderIdType getOrderId() const;
    DTO::Types::AmountType getAmount() const;
    DTO::Types::PriceType getPrice() const;
};

}
}
#endif // OrderChangeMSG_H
