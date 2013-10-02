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

Types::Message::MessageType Request::getType(QDataStream& in)
{
    if(in.device()->bytesAvailable() < 1)
    {
        qDebug() <<"[" << getMessageName() << "] getType : Pusty strumień.";

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

    Types::Message::MessageType requestType;
    in >> requestType;

    if(requestType != type())
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
