#ifndef CANCELORDERMSG_H
#define CANCELORDERMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
class NETWORKPROTOCOLSHARED_EXPORT CancelOrderMsg : public Request
{
    DTO::Types::OrderIdType m_orderId;

public:
    CancelOrderMsg(QDataStream& in);

    DTO::Types::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;
    DTO::Types::OrderIdType getOrderId() const;

    static const QString getMessageName();
};
}
}
#endif // CANCELORDERMSG_H
