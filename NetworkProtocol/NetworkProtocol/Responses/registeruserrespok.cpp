#include "registeruserrespok.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

RegisterUserRespOk::RegisterUserRespOk(Types::UserIdType userId)
    : m_userId(userId)
{
}

Types::MessageLengthType RegisterUserRespOk::length() const
{ 
    return sizeof(Types::Message::MessageType) +
           sizeof(m_userId);
}

Types::Message::MessageType RegisterUserRespOk::type() const
{
    return Types::Message::MessageType::RESPONSE_REGISTER_USER_OK;
}

void RegisterUserRespOk::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);
    out << m_userId;
}

Types::UserIdType RegisterUserRespOk::getUserId() const
{
    return m_userId;
}

const QString RegisterUserRespOk::getMessageName()
{
    return Response::getMessageName() + " :: RegisterUserRespOk";
}

}
}
