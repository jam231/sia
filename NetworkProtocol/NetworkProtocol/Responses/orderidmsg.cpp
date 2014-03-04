#include "orderidmsg.h"

#include <utilities.h>

#include <stdexcept>

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

OrderId::OrderId(Types::OrderIdType orderId) : _orderId(orderId)
{
    if(_orderId <= 0)
    {
        GLOBAL_LOG_TRACE(QString("Invalid argument: orderId(%1) <= 0")
                  .arg(_orderId.value));
        throw std::invalid_argument("orderId <= 0.");
    }
}

Types::Message::MessageLengthType OrderId::length() const
{
    return sizeof(_orderId);
}

Types::Message::MessageType OrderId::type() const
{
    return Types::Message::MessageType::RESPONSE_ORDER_ID;
}

void OrderId::send(QIODevice* connection)
{
    // Domyślnie BigEndian.
    QDataStream out(connection);

    sendHeader(out);
    out << _orderId;
}



}
}
