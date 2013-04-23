#ifndef LOGINUSERMSG_H
#define LOGINUSERMSG_H

#include "imessage.h"

#include <QIODevice>

class LoginUserMsg : public IMessage
{
    qint32 m_userId;
public:
    LoginUserMsg(QIODevice* msg);

    MessageType type() const;
    qint32 userId() const;
};

#endif // LOGINUSERMSG_H
