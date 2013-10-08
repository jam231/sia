#ifndef ORDERIDMSG_H
#define ORDERIDMSG_H

#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT OrderId : public Response
{
    DTO::Types::OrderIdType m_orderId;

public:
    OrderId(DTO::Types::OrderIdType orderId);

    DTO::Types::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

    static const QString getMessageName();

    void send(QIODevice *connection);
};

}
}
#endif // ORDERIDMSG_H
