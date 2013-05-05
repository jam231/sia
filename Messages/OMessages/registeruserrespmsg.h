#ifndef REGISTERUSERRESPMSG_H
#define REGISTERUSERRESPMSG_H

#include "omessage.h"

#include <QIODevice>

class RegisterUserRespMsg : public OMessage
{
    qint32 m_userId;

    qint32 length() const;
public:
    RegisterUserRespMsg(qint32 userId);

    MessageType type() const;
    qint32 getUserId() const;
    void send(QIODevice* connection);
};

#endif // REGISTERUSERRESPMSG_H
