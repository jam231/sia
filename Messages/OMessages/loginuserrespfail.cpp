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
    QDataStream out(connection);
    out.setByteOrder(QDataStream::BigEndian);

    out << static_cast<qint8>(type());
    out << m_reason;
}
qint32 LoginUserRespFail::length() const
{
    /*
     * Bardzo to naiwne, ale niech narazie zostanie.
     *      --jam231
     */
    return m_reason.size() + sizeof(type());
}
