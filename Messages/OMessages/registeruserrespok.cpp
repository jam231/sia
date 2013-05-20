#include "registeruserrespok.h"

#include <QDataStream>

RegisterUserRespOk::RegisterUserRespOk(qint32 userId) : OMessage(),
                                                          m_userId(userId)
{
}

qint32 RegisterUserRespOk::length() const
{ 
    return sizeof(m_userId) + sizeof(type());
}

IOMessage::MessageType RegisterUserRespOk::type() const
{
    return REGISTER_USER_RESP_OK;
}

void RegisterUserRespOk::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    // Nie wiem czemu trzeba tu robić cast'a
    // IOMessage:MessageType to enum, który ma typ qint8
    // jednak bez cast'a strumien traktuje type() jako 4 bajty.
    out << static_cast<qint16>(sizeof(qint8) + sizeof(m_userId))
        << static_cast<qint8>(type())
        << m_userId;
}

qint32 RegisterUserRespOk::getUserId() const
{
    return m_userId;
}
