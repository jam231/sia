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

    qint16 length() const;
    MessageType type() const;
    QString getPassword() const;
};
}
}
#endif // REGISTERUSERREQMSG_H
