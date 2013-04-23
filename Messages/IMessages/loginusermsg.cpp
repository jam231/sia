#include "loginusermsg.h"

#include <QDataStream>

LoginUserMsg::LoginUserMsg(QIODevice* msg) : IMessage()
{
    QDataStream tmpStream;
    tmpStream>>m_userId;
}

IOMessage::MessageType LoginUserMsg::type() const
{
    return LOGIN_USER;
}

qint32 LoginUserMsg::userId() const
{
    return m_userId;
}
