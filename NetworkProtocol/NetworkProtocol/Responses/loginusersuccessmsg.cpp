#include "loginusersuccessmsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

LoginUserSuccess::LoginUserSuccess()
{
}

Types::Message::MessageType LoginUserSuccess::type() const
{
    return Types::Message::MessageType::RESPONSE_LOGIN_USER_FAILURE;
}

void LoginUserSuccess::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);
}

Types::MessageLengthType LoginUserSuccess::length() const
{
    return sizeof(Types::Message::MessageType);
}

const QString LoginUserSuccess::getMessageName()
{
    return Response::getMessageName() + " :: LoginUserSuccess";
}

}
}
