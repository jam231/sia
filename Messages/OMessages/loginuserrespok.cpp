#include "loginuserrespok.h"

#include <QDataStream>

LoginUserRespOk::LoginUserRespOk()
{
}
IOMessage::MessageType LoginUserRespOk::type() const
{
    return LOGIN_USER_RESP_OK;
}
void LoginUserRespOk::send(QIODevice* connection)
{
    QDataStream out(connection);
    out.setByteOrder(QDataStream::BigEndian);

    out << static_cast<qint8>(type());
}

qint32 LoginUserRespOk::length() const
{
    return  sizeof(type());
}
