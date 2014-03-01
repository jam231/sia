#ifndef GETMYORDERMSG_H
#define GETMYORDERMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
/// TODO:   Add description with serialization format.
class NETWORKPROTOCOLSHARED_EXPORT GetMyOrders : public Request
{
public:
    GetMyOrders(QDataStream& in);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

};
}
}
#endif // GETMYORDERMSG_H
