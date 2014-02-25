#ifndef BUYTRANSACTIONMSG_H
#define BUYTRANSACTIONMSG_H

#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT BuyTransaction : public Response
{
    DTO::Types::AmountType m_amount;
    DTO::Types::OrderIdType m_orderId;

public:
    BuyTransaction(DTO::Types::OrderIdType orderId, DTO::Types::AmountType amount);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;


    void send(QIODevice *connection);
};

}
}
#endif // BUYTRANSACTIONMSG_H
