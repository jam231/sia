#include "registeruserrespmsg.h"

#include <QDataStream>

RegisterUserRespMsg::RegisterUserRespMsg(qint32 userId) : OMessage(),
                                                          m_userId(userId)
{
}

qint32 RegisterUserRespMsg::length() const
{ 
    return sizeof(m_userId) + sizeof(type());
}

IOMessage::MessageType RegisterUserRespMsg::type() const
{
    return REGISTER_USER_RESP;
}

void RegisterUserRespMsg::send(QIODevice* connection)
{
    QDataStream out(connection);
    out.setByteOrder(QDataStream::BigEndian);

    out << static_cast<qint8>(type());
    out << m_userId;
}

qint32 RegisterUserRespMsg::getUserId() const
{
    return m_userId;
}
