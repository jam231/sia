#ifndef LOGINUSERMSG_H
#define LOGINUSERMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
class NETWORKPROTOCOLSHARED_EXPORT  LoginUserReqMsg : public Request
{
    DTO::Types::UserIdType m_userId;
    QString m_password;

public:
    LoginUserReqMsg(QDataStream& in);

    DTO::Types::MessageLengthType length() const;

    DTO::Types::MessageType type() const;
    DTO::Types::UserIdType getUserId() const;
    QString getUserPassword() const;

    static const QString getMessageName();
};
}
}
#endif // LOGINUSERMSG_H
