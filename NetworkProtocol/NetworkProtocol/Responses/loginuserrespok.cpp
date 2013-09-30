#include "loginuserrespok.h"

namespace NetworkProtocol
{
namespace Responses
{

LoginUserRespOk::LoginUserRespOk()
{
}

Message::MessageType LoginUserRespOk::type() const
{
    return LOGIN_USER_RESP_OK;
}
void LoginUserRespOk::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);
}

qint16 LoginUserRespOk::length() const
{
    return  sizeof(MessageType);
}

}
}
