#ifndef REGISTERUSERREQMSG_H
#define REGISTERUSERREQMSG_H

#include "imessage.h"

class RegisterUserReqMsg : public IMessage
{
    double m_cash;
public:
    RegisterUserReqMsg(QIODevice* msg);

    MessageType type() const;
    double cash() const;
};

#endif // REGISTERUSERREQMSG_H
