#include "okmsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

Ok::Ok()
{
}

Types::Message::MessageType Ok::type() const
{
    return Types::Message::RESPONSE_OK;
}

void Ok::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);
}

Types::Message::MessageLengthType Ok::length() const
{
    return sizeof(Types::Message::MessageType);
}


}
}
