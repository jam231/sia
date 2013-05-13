#include "registeruserrespfail.h"


#include <QDataStream>


RegisterUserRespFail::RegisterUserRespFail(QString reason) : m_reason(reason)
{
}

qint32 RegisterUserRespFail::length() const
{
    return m_reason.length() + sizeof(type());
}

IOMessage::MessageType RegisterUserRespFail::type() const
{
    return REGISTER_USER_RESP_FAIL;
}
#include <QDebug>
#include <string.h>
void RegisterUserRespFail::send(QIODevice* connection)
{
    // Domyślnie BigEndian.
    QDataStream out(connection);

    // Nie wiem czemu trzeba tu robić cast'a
    // IOMessage:MessageType to enum, który ma typ qint8
    // jednak bez cast'a strumien traktuje type() jako 4 bajty.
    out << static_cast<qint8>(type());
    auto reason = m_reason.toUtf8();
    out << reason.size();
    connection->write(m_reason.toUtf8());
}