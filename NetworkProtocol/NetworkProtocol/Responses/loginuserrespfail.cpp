#include "loginuserrespfail.h"

namespace NetworkProtocol
{
namespace Responses
{

LoginUserRespFail::LoginUserRespFail(QString reason)
{
    m_reason = reason;
}

Message::MessageType LoginUserRespFail::type() const
{
    return LOGIN_USER_RESP_FAIL;
}
void LoginUserRespFail::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    auto reason = m_reason.toUtf8();

    sendHeader(out);
    out << static_cast<qint16>(reason.size());
    connection->write(reason);
}
qint16 LoginUserRespFail::length() const
{
    /*
     * Bardzo to naiwne, ale niech narazie zostanie.
     *      --jam231
     */
    return m_reason.size() + sizeof(MessageType) + sizeof(qint16);
}

}
}
