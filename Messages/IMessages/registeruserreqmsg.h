#ifndef REGISTERUSERREQMSG_H
#define REGISTERUSERREQMSG_H

#include <QString>

#include "imessage.h"



class RegisterUserReqMsg : public IMessage
{
    QString m_password;
public:
    RegisterUserReqMsg(QIODevice* msg);

    MessageType type() const;
    QString getPassword() const;
};

#endif // REGISTERUSERREQMSG_H
