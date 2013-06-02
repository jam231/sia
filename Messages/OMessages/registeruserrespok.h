#ifndef REGISTERUSERRESPMSG_H
#define REGISTERUSERRESPMSG_H

#include "omessage.h"

#include <QIODevice>

class RegisterUserRespOk : public OMessage
{
    qint32 m_userId;

    qint16 length() const;
public:
    RegisterUserRespOk(qint32 userId);

    MessageType type() const;
    qint32 getUserId() const;
    void send(QIODevice* connection);
};

#endif // REGISTERUSERRESPMSG_H
