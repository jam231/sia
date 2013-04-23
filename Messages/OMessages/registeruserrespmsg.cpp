#include "registeruserrespmsg.h"

#include <QDataStream>

RegisterUserRespMsg::RegisterUserRespMsg(qint32 userId) : OMessage(), m_userId(userId)
{
}

qint32 RegisterUserRespMsg::length() const
{
    //8 bajtów na typ i dlugosc
    return 8 + sizeof(RegisterUserRespMsg);
}

IOMessage::MessageType RegisterUserRespMsg::type() const
{
    return REGISTER_USER_RESP;
}

void RegisterUserRespMsg::send(QIODevice* connection)
{
    QDataStream tmpStream(connection);
    tmpStream<<length();
    tmpStream<<static_cast<qint32>(type());
    tmpStream<<m_userId;
}
