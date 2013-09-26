#ifndef REGISTERUSERREQMSG_H
#define REGISTERUSERREQMSG_H

#include "imessage.h"

#include <QString>

namespace NetworkProtocol {

class NETWORKPROTOCOLSHARED_EXPORT RegisterUserReqMsg : public IMessage
{
    QString m_password;
    qint16 length() const;
public:
    RegisterUserReqMsg(QDataStream& msg);

    MessageType type() const;
    QString getPassword() const;
};

}
#endif // REGISTERUSERREQMSG_H
