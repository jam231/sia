#ifndef BUYTRANSACTIONMSG_H
#define BUYTRANSACTIONMSG_H

#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT BuyTransactionMsg : public Response
{
    DTO::Types::OrderIdType m_amount;
    DTO::Types::AmountType m_orderId;
public:
    BuyTransactionMsg(DTO::Types::OrderIdType orderId, DTO::Types::AmountType amount);

    DTO::Types::MessageLengthType length() const;
    void send(QIODevice *connection);
    Message::MessageType type() const;
};

}
}
#endif // BUYTRANSACTIONMSG_H
