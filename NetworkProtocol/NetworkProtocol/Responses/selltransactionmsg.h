#ifndef SELLTRANSACTIONMSG_H
#define SELLTRANSACTIONMSG_H

#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT SellTransactionMsg : public Response
{
    DTO::Types::AmountType m_amount;
    DTO::Types::OrderIdType m_orderId;

public:
    SellTransactionMsg(DTO::Types::OrderIdType m_orderId, DTO::Types::AmountType m_amount);

    DTO::Types::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

    static const QString getMessageName();

    void send(QIODevice *connection);
};

}
}
#endif // SELLTRANSACTIONMSG_H
