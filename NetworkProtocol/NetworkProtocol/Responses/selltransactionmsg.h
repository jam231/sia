#ifndef SELLTRANSACTIONMSG_H
#define SELLTRANSACTIONMSG_H

#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT SellTransaction : public Response
{
    DTO::Types::AmountType m_amount;
    DTO::Types::OrderIdType m_orderId;

public:
    SellTransaction(DTO::Types::OrderIdType m_orderId, DTO::Types::AmountType m_amount);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

    void send(QIODevice *connection);
};

}
}
#endif // SELLTRANSACTIONMSG_H
