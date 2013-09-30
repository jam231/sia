#ifndef LOGINUSERMSG_H
#define LOGINUSERMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
class NETWORKPROTOCOLSHARED_EXPORT  LoginUserReqMsg : public Request
{
    qint32 m_userId;
    QString m_password;

public:
    LoginUserReqMsg(QDataStream& in);

    qint16 length() const;

    MessageType type() const;
    qint32 getUserId() const;
    QString getUserPassword() const;
};
}
}
#endif // LOGINUSERMSG_H
