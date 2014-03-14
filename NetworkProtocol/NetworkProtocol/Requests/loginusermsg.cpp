#include "loginusermsg.h"

#include "networkprotocol_utilities.h"

namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

LoginUser::LoginUser(QDataStream& serialized_request)
    : LoginUser(std::move(GlobalUtilities::getLogger()), serialized_request)
{}

LoginUser::LoginUser(std::shared_ptr<AbstractLogger> logger,
                     QDataStream &serialized_request)
{
    Types::Message::MessageLengthType password_length;
    if(serialized_request.device()->bytesAvailable() < sizeof(password_length) +
                                                       sizeof(_userId))
    {
        LOG_TRACE(logger,
                  QString("Malformed request: Not enough bytes in serialized_request"\
                          " to read userId and password length. Is %1 should be >%2.")
                  .arg(serialized_request.device()->bytesAvailable())
                  .arg(sizeof(password_length) + sizeof(_userId)));

        throw MalformedRequest("Not enough bytes in serialized_request to read"\
                                   " user id and password length.");
    }

    serialized_request >> _userId;
    serialized_request >> password_length;

    if(password_length <= 4 || _userId <= 0)
    {
        LOG_TRACE(logger,
                  QString("Invalid password length or userId. "\
                          "password length(%1) <= 4 || _userId(%2) <= 0.")
                  .arg(password_length)
                  .arg(_userId.value));
        throw InvalidRequestBody("Password length <= 4 or user id <= 0.");
    }
    if(serialized_request.device()->bytesAvailable() != password_length)
    {
        LOG_TRACE(logger,
                  QString("Invalid request body: Wrong number of bytes in stream."\
                          " Is %1 should be %2.")
                  .arg(serialized_request.device()->bytesAvailable())
                  .arg(password_length));
        throw MalformedRequest("Wrong number of bytes in serialized_request"\
                               "for password.");
    }
    QByteArray buffer(password_length, Qt::Uninitialized);

    serialized_request.readRawData(buffer.data(), password_length);
    _password = QString(buffer);
}

Types::Message::MessageType LoginUser::type() const
{
    return Types::Message::MessageType::REQUEST_LOGIN_USER;
}

Types::UserIdType LoginUser::getUserId() const
{
    return _userId;
}

QString LoginUser::getUserPassword() const
{
    return _password;
}

Types::Message::MessageLengthType LoginUser::length() const
{
    return Request::length() + sizeof(_userId) +
           sizeof(Types::Message::MessageLengthType) + _password.toUtf8().size();
}


}
}
