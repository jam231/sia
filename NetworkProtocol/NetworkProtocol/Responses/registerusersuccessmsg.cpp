#include "registerusersuccessmsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

RegisterUserSuccess::RegisterUserSuccess(Types::UserIdType userId)
    : m_userId(userId)
{
}

Types::Message::MessageLengthType RegisterUserSuccess::length() const
{ 
    return sizeof(Types::Message::MessageType) +
           sizeof(m_userId);
}

Types::Message::MessageType RegisterUserSuccess::type() const
{
    return Types::Message::MessageType::RESPONSE_REGISTER_USER_SUCCESS;
}

void RegisterUserSuccess::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);
    out << m_userId;
}

Types::UserIdType RegisterUserSuccess::getUserId() const
{
    return m_userId;
}

const QString RegisterUserSuccess::getMessageName()
{
    return Response::getMessageName() + " :: RegisterUserSuccess";
}

}
}
