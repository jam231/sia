#include "registerusersuccessmsg.h"

#include <utilities.h>
#include <stdexcept>

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

RegisterUserSuccess::RegisterUserSuccess(Types::UserIdType userId)
    : RegisterUserSuccess(std::move(GlobalUtilities::getLogger()), userId)
{}

RegisterUserSuccess::RegisterUserSuccess(std::shared_ptr<AbstractLogger> logger,
                                         Types::UserIdType userId)
    : _userId(userId)
{
    if(_userId <= 0 )
    {
        LOG_TRACE(logger,
                  QString("Invalid argument: userId(%1) <= 0")
                  .arg(_userId.value));

        throw std::invalid_argument("userId <= 0.");
    }
}

Types::Message::MessageLengthType RegisterUserSuccess::length() const
{ 
    return Response::length() + sizeof(_userId);
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
    out << _userId;
}

Types::UserIdType RegisterUserSuccess::getUserId() const
{
    return _userId;
}


}
}
