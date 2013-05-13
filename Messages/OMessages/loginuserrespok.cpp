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
    // Domyślnie BigEndian
    QDataStream out(connection);
    // Nie wiem czemu trzeba tu robić cast'a
    // IOMessage:MessageType to enum, który ma typ qint8
    // jednak bez cast'a strumien traktuje type() jako 4 bajty.
    out << static_cast<qint8>(type());
}

qint32 LoginUserRespOk::length() const
{
    return  sizeof(type());
}
