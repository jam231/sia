#include "loginuserrespok.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

LoginUserRespOk::LoginUserRespOk()
{
}

Types::MessageType LoginUserRespOk::type() const
{
    return Types::MessageType::LOGIN_USER_RESP_OK;
}
void LoginUserRespOk::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);
}

Types::MessageLengthType LoginUserRespOk::length() const
{
    return sizeof(Types::MessageType);
}

}
}
