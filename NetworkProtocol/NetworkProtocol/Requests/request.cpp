#include "request.h"
#include <QDebug>

namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

Types::Message::MessageLengthType Request::getMessageLength(QIODevice* data)
{
    if(data->bytesAvailable() < 2)
        return -1;

    Types::Message::MessageLengthType msgLength;

    QDataStream in(data->peek(sizeof(Types::Message::MessageLengthType)));

    in >> msgLength;

    return msgLength;
}

Types::Message::MessageType Request::getType(QDataStream& in)
{
    if(in.device()->bytesAvailable() < 1)
    {
        qWarning() <<"[] Nie można odczytać typu wiadomości - Pusty strumień.";

        return Types::Message::MessageType::MESSAGE_UNDEFINED;
    }
    Types::Message::MessageType type;
    in >> type;

    return type;
}

Request::Request(QDataStream &in)
{
    validateRequest(in);
}

Request::Request()
{
}

void Request::validateRequest(QDataStream &in)
{
    if(in.device()->bytesAvailable() != length())
    {
        qWarning() <<"[] Niepoprawna długość wiadomości.\n"
                   << "Oczekiwano" << length() << "bajtów.\n"
                   << "Liczba bajtów dostępnych w buforze:"
                   << in.device()->bytesAvailable();
        throw InvalidRequest();
    }

    Types::Message::MessageType requestType;
    in >> requestType;

    if(requestType != type())
    {
        qWarning() <<"[] Niepoprawny typ wiadomości.\n"
                   << "Oczekiwano typu:" << type() << "\n"
                   << "Typ podany w wiadomości:" << requestType;
        throw InvalidRequest();
    }
}

}
}
