#include "registeruserrespok.h"

#include <QDataStream>

RegisterUserRespOk::RegisterUserRespOk(qint32 userId) : OMessage(),
                                                          m_userId(userId)
{
}

qint16 RegisterUserRespOk::length() const
{ 
    return sizeof(m_userId) + sizeof(MessageType);
}

IOMessage::MessageType RegisterUserRespOk::type() const
{
    return REGISTER_USER_RESP_OK;
}

void RegisterUserRespOk::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);
    out << m_userId;
}

qint32 RegisterUserRespOk::getUserId() const
{
    return m_userId;
}
