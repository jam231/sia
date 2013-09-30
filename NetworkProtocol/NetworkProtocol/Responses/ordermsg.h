#ifndef NEWTRANSACTION_H
#define NEWTRANSACTION_H

#include "response.h"

#include "DataTransferObjects/order.h"

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT OrderMsg : public Response
{
    DTO::Order m_order;

public:
    OrderMsg(DTO::Types::OrderIdType, DTO::Types::OrderType, DTO::Types::StockIdType,
             DTO::Types::AmountType, DTO::Types::PriceType);
    OrderMsg(DTO::Order order);
    // TODO:
    // Brzydze sie siebie, nalezy to usunac ! --jam231
    //OrderMsg();

    DTO::Types::MessageLengthType length() const;

    const DTO::Order& getOrder() const;

    void send(QIODevice *connection);
    Message::MessageType type() const;

    virtual ~OrderMsg() {}

};

}
}
#endif // NEWTRANSACTION_H
