#include "ordercompletedmsg.h"

#include <utilities.h>
#include <stdexcept>

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

OrderCompleted::OrderCompleted(Types::OrderIdType orderId)
    : _orderId(orderId)
{

    if(_orderId <= 0)
    {
        GLOBAL_LOG_TRACE(QString("Invalid arguments: orderId(%1) <= 0.")
                  .arg(_orderId.value));
        throw std::invalid_argument("orderId <= 0.");
    }
}

void OrderCompleted::send(QIODevice *connection)
{
    QDataStream out(connection);

    sendHeader(out);
    out  << _orderId;
}

Types::Message::MessageType OrderCompleted::type() const
{
    return Types::Message::MessageType::RESPONSE_ORDER_COMPLETED;
}

Types::Message::MessageLengthType OrderCompleted::length() const
{
    return Response::length() + sizeof(_orderId);
}


}
}
