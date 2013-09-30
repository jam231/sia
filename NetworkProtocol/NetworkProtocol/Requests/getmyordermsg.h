#ifndef GETMYORDERMSG_H
#define GETMYORDERMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
class NETWORKPROTOCOLSHARED_EXPORT GetMyOrderMsg : public Request
{
public:
    GetMyOrderMsg(QDataStream& in);

    DTO::Types::MessageLengthType length() const;
    DTO::Types::MessageType type() const;

    static const QString getMessageName();
};
}
}
#endif // GETMYORDERMSG_H
