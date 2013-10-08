#ifndef SHOWORDERMSG_H
#define SHOWORDERMSG_H

#include "response.h"

#include "DataTransferObjects/order.h"

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT ShowOrder : public Response
{
    DTO::Order m_order;

public:
    ShowOrder(DTO::Types::OrderIdType, DTO::Types::Order::OrderType, DTO::Types::StockIdType,
          DTO::Types::AmountType, DTO::Types::PriceType);
    ShowOrder(DTO::Order order);

    DTO::Types::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

    const DTO::Order& getOrder() const;

    static const QString getMessageName();

    void send(QIODevice *connection);

};

}
}
#endif // SHOWORDERMSG_H
