#ifndef REGISTERUSERREQMSG_H
#define REGISTERUSERREQMSG_H

#include "request.h"

#include <QString>

namespace NetworkProtocol {
namespace Requests
{
class NETWORKPROTOCOLSHARED_EXPORT RegisterUserReqMsg : public Request
{
    QString m_password;
public:
    RegisterUserReqMsg(QDataStream& msg);

    DTO::Types::MessageLengthType length() const;
    DTO::Types::MessageType type() const;
    QString getPassword() const;
};
}
}
#endif // REGISTERUSERREQMSG_H
