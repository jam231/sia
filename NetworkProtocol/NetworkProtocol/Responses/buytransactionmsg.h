#ifndef BUYTRANSACTIONMSG_H
#define BUYTRANSACTIONMSG_H

#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT BuyTransactionMsg : public Response
{
    DTO::Types::AmountType m_amount;
    DTO::Types::OrderIdType m_orderId;

public:
    BuyTransactionMsg(DTO::Types::OrderIdType orderId, DTO::Types::AmountType amount);

    DTO::Types::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

    static const QString getMessageName();

    void send(QIODevice *connection);
};

}
}
#endif // BUYTRANSACTIONMSG_H
