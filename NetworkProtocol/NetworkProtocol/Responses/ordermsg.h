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

    DTO::Types::MessageLengthType length() const;
    DTO::Types::MessageType type() const;

    const DTO::Order& getOrder() const;

    static const QString getMessageName();

    void send(QIODevice *connection);

};

}
}
#endif // NEWTRANSACTION_H
