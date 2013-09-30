#include "request.h"
#include <QDebug>

namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

Types::MessageLengthType Request::getMessageLength(QIODevice* data)
{
    if(data->bytesAvailable() < 2)
        return -1;

    Types::MessageLengthType msgLength;

    QDataStream in(data->peek(sizeof(Types::MessageLengthType)));

    in >> msgLength;

    return msgLength;
}

Message::MessageType Request::getType(QDataStream& in)
{
    if(in.device()->bytesAvailable() < 1)
    {
        qDebug() <<"[" << getMessageName() << "] getType -> Pusty strumień.";

        return Message::UNDEFINED;
    }
    Types::MessageTypeType type;
    in >> type;

    return toMessageType(type);
}

Request::Request(QDataStream &in)
{
    validateRequest(in);
}

Request::Request()
{
}

QString Request::getMessageName()
{
    return Message::getMessageName() + " :: Request";
}

void Request::validateRequest(QDataStream &in)
{
    if(in.device()->bytesAvailable() != length())
        throw InvalidRequest();

    Types::MessageTypeType requestType;
    in >> requestType;

    if(Message::toMessageType(requestType) != type())
    {
        qDebug() <<"[" << getMessageName() << "] Niepoprawny typ wiadomości oczekiwano"
               << type() << '\n'
               << "Otrzymano"
               << requestType;

        throw InvalidRequest();
    }
}

}
}
