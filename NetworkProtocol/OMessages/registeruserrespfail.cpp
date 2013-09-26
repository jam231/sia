#include "registeruserrespfail.h"

namespace NetworkProtocol {

RegisterUserRespFail::RegisterUserRespFail(QString reason) : m_reason(reason)
{
    m_reason = reason;
}

qint16 RegisterUserRespFail::length() const
{
    return sizeof(MessageType) + sizeof(qint16) + m_reason.size();
}

IOMessage::MessageType RegisterUserRespFail::type() const
{
    return REGISTER_USER_RESP_FAIL;
}
void RegisterUserRespFail::send(QIODevice* connection)
{
    // Domyślnie BigEndian.
    QDataStream out(connection);

    QByteArray reason = m_reason.toUtf8();

    sendHeader(out);
    out << static_cast<qint16>(reason.size());
    connection->write(reason);
}

}
