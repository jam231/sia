#ifndef CANCELORDERMSG_H
#define CANCELORDERMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
/// TODO:   Add description with serialization format.
class NETWORKPROTOCOLSHARED_EXPORT CancelOrder : public Request
{
    DTO::Types::OrderIdType _orderId;

public:
    CancelOrder(QDataStream& in);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;
    DTO::Types::OrderIdType getOrderId() const;

};
}
}
#endif // CANCELORDERMSG_H
