#ifndef LOGINUSERMSG_H
#define LOGINUSERMSG_H

#include "imessage.h"

namespace NetworkProtocol {

class NETWORKPROTOCOLSHARED_EXPORT  LoginUserReqMsg : public IMessage
{
    qint32 m_userId;
    QString m_password;

    qint16 length() const;
public:
    LoginUserReqMsg(QDataStream& in);

    MessageType type() const;
    qint32 getUserId() const;
    QString getUserPassword() const;

};

}
#endif // LOGINUSERMSG_H
