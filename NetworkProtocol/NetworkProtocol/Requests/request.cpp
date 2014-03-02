#include "request.h"
#include <QDebug>

#include "utilities.h"

namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;
using namespace Types;
/*
Request::Request(QDataStream &in)
{
    validateRequest(in);
}
*/
Request::Request()
{
}

/*
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

    Message::MessageType requestType;
    in >> requestType;

    if(requestType != type())
    {
        qWarning() <<"[] Niepoprawny typ wiadomości.\n"
                   << "Oczekiwano typu:" << type() << "\n"
                   << "Typ podany w wiadomości:" << requestType;
        throw InvalidRequest("Invalid message type.");
    }
}
*/
}
}
