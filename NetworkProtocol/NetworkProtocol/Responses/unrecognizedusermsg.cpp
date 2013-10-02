#include "unrecognizedusermsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

UnrecognizedUserMsg::UnrecognizedUserMsg()
{
}

Types::Message::MessageType UnrecognizedUserMsg::type() const
{
    return Types::Message::MessageType::UNRECOGNIZED_USER;
}

Types::MessageLengthType UnrecognizedUserMsg::length() const
{

    return sizeof(Types::Message::MessageType);
}

void UnrecognizedUserMsg::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);
    sendHeader(out);
}

const QString UnrecognizedUserMsg::getMessageName()
{
    return Response::getMessageName() + " :: UnrecognizedUserMsg";
}

}
}
