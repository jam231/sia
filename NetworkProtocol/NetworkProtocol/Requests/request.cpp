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

Types::MessageType Request::getType(QDataStream& in)
{
    if(in.device()->bytesAvailable() < 1)
    {
        qDebug() <<"[" << getMessageName() << "] getType : Pusty strumień.";

        return Types::MessageType::MESSAGE_UNDEFINED;
    }
    Types::MessageTypeType type;
    in >> type;

    return Types::toMessageType(type);
}

Request::Request(QDataStream &in)
{
    validateRequest(in);
}

Request::Request()
{
}

const QString Request::getMessageName()
{
    return Message::getMessageName() + " :: Request";
}

void Request::validateRequest(QDataStream &in)
{
    if(in.device()->bytesAvailable() != length())
    {
        qDebug() <<"[" << getMessageName() << "] Niepoprawny długość wiadomości. Oczekiwano długości ="
                << length() << '\n'
               << "Otrzymana wiadomość ma długość = "
               << in.device()->bytesAvailable();

        throw InvalidRequest();
    }

    Types::MessageTypeType requestType;
    in >> requestType;

    if(Types::toMessageType(requestType) != type())
    {
        qDebug() <<"[" << getMessageName() << "] Niepoprawny typ wiadomości. Oczekiwano"
               << type() << '\n'
               << "Otrzymano"
               << requestType;

        throw InvalidRequest();
    }
}

}
}
