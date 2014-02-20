#ifndef CANCELORDERMSG_H
#define CANCELORDERMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
class NETWORKPROTOCOLSHARED_EXPORT CancelOrder : public Request
{
    DTO::Types::OrderIdType m_orderId;

public:
    CancelOrder(QDataStream& in);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;
    DTO::Types::OrderIdType getOrderId() const;

    static const QString getMessageName();
};
}
}
#endif // CANCELORDERMSG_H
