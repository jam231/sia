#include "unrecognizedusermsg.h"

namespace NetworkProtocol
{
namespace Responses
{

UnrecognizedUserMsg::UnrecognizedUserMsg()
{
}

Message::MessageType UnrecognizedUserMsg::type() const
{
    return UNRECOGNIZED_USER;
}

qint16 UnrecognizedUserMsg::length() const
{

    return sizeof(MessageType);
}

void UnrecognizedUserMsg::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);
    sendHeader(out);
}

}
}
