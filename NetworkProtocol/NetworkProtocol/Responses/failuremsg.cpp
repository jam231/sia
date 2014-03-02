#include "failuremsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

Failure::Failure(Types::Failure::FailureType reason)
{
    _reason = reason;
}

Types::Message::MessageType Failure::type() const
{
    return Types::Message::RESPONSE_FAILURE;
}

void Failure::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);

    out << _reason;
}

Types::Message::MessageLengthType Failure::length() const
{
    return sizeof(_reason);
}

}
}
