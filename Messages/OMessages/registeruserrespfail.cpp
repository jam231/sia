#include "registeruserrespfail.h"

#include <QDataStream>


RegisterUserRespFail::RegisterUserRespFail(QString reason) : m_reason(reason)
{
    m_reason = reason;
}

qint16 RegisterUserRespFail::length() const
{
    return m_reason.length() + sizeof(type());
}

IOMessage::MessageType RegisterUserRespFail::type() const
{
    return REGISTER_USER_RESP_FAIL;
}
void RegisterUserRespFail::send(QIODevice* connection)
{
    // Domyślnie BigEndian.
    QDataStream out(connection);

    // Nie wiem czemu trzeba tu robić cast'a
    // IOMessage:MessageType to enum, który ma typ qint8
    // jednak bez cast'a strumien traktuje type() jako 4 bajty.

    auto reason = m_reason.toUtf8();
    out << static_cast<qint16>(sizeof(qint8) + sizeof(qint16) + reason.size())
        << static_cast<qint8>(type())
        << static_cast<qint16>(reason.size());
    connection->write(reason);
}
