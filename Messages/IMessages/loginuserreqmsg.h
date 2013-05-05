#ifndef LOGINUSERMSG_H
#define LOGINUSERMSG_H

#include "imessage.h"

#include <QIODevice>

class LoginUserReqMsg : public IMessage
{
    qint32 m_userId;
    QString m_password;
public:
    LoginUserReqMsg(QIODevice* msg);

    MessageType type() const;
    qint32 getUserId() const;
    QString getUserPassword() const;

};

#endif // LOGINUSERMSG_H
