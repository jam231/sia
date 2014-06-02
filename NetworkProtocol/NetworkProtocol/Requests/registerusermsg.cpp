#include "registerusermsg.h"

#include "networkprotocol_utilities.h"

namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

RegisterUser::RegisterUser(QDataStream& serialized_request)
    : RegisterUser(std::move(GlobalUtilities::getLogger()), serialized_request)
{}

RegisterUser::RegisterUser(std::shared_ptr<AbstractLogger> logger,
                           QDataStream& serialized_request)
{
    Types::Message::MessageLengthType password_length;
    if(serialized_request.device()->bytesAvailable() < sizeof(password_length))
    {
        LOG_TRACE(logger,
                  QString("Malformed request: Not enough bytes in serialized_request"\
                          " to read password length. Is %1 should be >%2.")
                  .arg(serialized_request.device()->bytesAvailable())
                  .arg(sizeof(password_length)));

        throw MalformedRequestError("Not enough bytes in serialized_request to read"\
                               " password length.");
    }

    serialized_request >> password_length;
    if(password_length <= 4)
    {
        LOG_TRACE(logger,
                  QString("Invalid password length. Is %1 should be > 4.")
                  .arg(password_length));

        throw InvalidRequestBodyError("Password is <= 4.");
    }
    if(serialized_request.device()->bytesAvailable() != password_length)
    {
        LOG_TRACE(logger,
                  QString("Invalid request body: Wrong number of bytes in stream."\
                          " Is %1 should be %2.")
                  .arg(serialized_request.device()->bytesAvailable())
                  .arg(password_length));

        throw MalformedRequestError("Wrong number of bytes in serialized_request"\
                               "for password.");
    }

    QByteArray buffer(password_length, Qt::Uninitialized);
    serialized_request.readRawData(buffer.data(), password_length);
    _password = QString(buffer);
}

Types::Message::MessageType RegisterUser::type() const
{
    return Types::Message::MessageType::REQUEST_REGISTER_USER;
}

QString RegisterUser::getPassword() const
{
    return _password;
}


DTO::Types::Message::MessageLengthType RegisterUser::length() const
{
    return Request::length() +
           sizeof(Types::Message::MessageLengthType) + _password.toUtf8().size();
}

}
}
