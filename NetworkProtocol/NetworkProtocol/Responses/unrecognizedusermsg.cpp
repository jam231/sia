#include "unrecognizedusermsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

UnrecognizedUser::UnrecognizedUser()
{
}

Types::Message::MessageType UnrecognizedUser::type() const
{
    return Types::Message::MessageType::RESPONSE_UNRECOGNIZED_USER;
}

Types::Message::MessageLengthType UnrecognizedUser::length() const
{

    return sizeof(Types::Message::MessageType);
}

void UnrecognizedUser::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);
    sendHeader(out);
}

const QString UnrecognizedUser::getMessageName()
{
    return Response::getMessageName() + " :: UnrecognizedUser";
}

}
}
