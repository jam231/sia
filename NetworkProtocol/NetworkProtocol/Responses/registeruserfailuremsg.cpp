#include "registeruserfailuremsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

RegisterUserFailure::RegisterUserFailure(QString reason)
    : m_reason(reason)
{
    m_reason = reason;
}

Types::Message::MessageLengthType RegisterUserFailure::length() const
{
    return sizeof(Types::Message::MessageType) +
           sizeof(Types::Message::MessageLengthType) + m_reason.toUtf8().size();
}

Types::Message::MessageType RegisterUserFailure::type() const
{
    return Types::Message::MessageType::RESPONSE_REGISTER_USER_FAILURE;
}

void RegisterUserFailure::send(QIODevice* connection)
{
    // Domyślnie BigEndian.
    QDataStream out(connection);

    QByteArray reason_bytes = m_reason.toUtf8();

    sendHeader(out);

    out << static_cast<Types::Message::MessageLengthType>(reason_bytes.size());
    connection->write(reason_bytes);
}

const QString RegisterUserFailure::getMessageName()
{
    return Response::getMessageName() + " :: RegisterUserFailure";
}

}
}
