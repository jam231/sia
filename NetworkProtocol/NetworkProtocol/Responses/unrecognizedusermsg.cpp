#include "unrecognizedusermsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

UnrecognizedUserMsg::UnrecognizedUserMsg()
{
}

Types::MessageType UnrecognizedUserMsg::type() const
{
    return Types::MessageType::UNRECOGNIZED_USER;
}

Types::MessageLengthType UnrecognizedUserMsg::length() const
{

    return sizeof(Types::MessageType);
}

void UnrecognizedUserMsg::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);
    sendHeader(out);
}

}
}
