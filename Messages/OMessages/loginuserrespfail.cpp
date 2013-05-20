#include "loginuserrespfail.h"

#include <QDataStream>

LoginUserRespFail::LoginUserRespFail(QString reason)
{
    m_reason = reason;
}

IOMessage::MessageType LoginUserRespFail::type() const
{
    return LOGIN_USER_RESP_FAIL;
}
void LoginUserRespFail::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    // Nie wiem czemu trzeba tu robić cast'a
    // IOMessage:MessageType to enum, który ma typ qint8
    // jednak bez cast'a strumien traktuje type() jako 4 bajty.
    auto reason = m_reason.toUtf8();
    out << static_cast<qint16>(sizeof(qint8) + sizeof(qint16) + reason.size())
        << static_cast<qint8>(type())
        << static_cast<qint16>(reason.size());
    connection->write(m_reason.toUtf8());
}
qint32 LoginUserRespFail::length() const
{
    /*
     * Bardzo to naiwne, ale niech narazie zostanie.
     *      --jam231
     */
    return m_reason.size() + sizeof(type());
}
