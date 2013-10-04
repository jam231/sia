#include "loginuserfailuremsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

LoginuserFailure::LoginuserFailure(QString reason)
{
    m_reason = reason;
}

Types::Message::MessageType LoginuserFailure::type() const
{
    return Types::Message::MessageType::RESPONSE_LOGIN_USER_FAILURE;
}

void LoginuserFailure::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    QByteArray reason_bytes = m_reason.toUtf8();

    sendHeader(out);
    out << static_cast<Types::MessageLengthType>(reason_bytes.size());
    connection->write(reason_bytes);
}

Types::MessageLengthType LoginuserFailure::length() const
{
    return sizeof(Types::Message::MessageType) +
           m_reason.toUtf8().size()  + sizeof(Types::MessageLengthType);
}

const QString LoginuserFailure::getMessageName()
{
    return Response::getMessageName() + " :: LoginUserFailure";
}

}
}
