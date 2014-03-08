#include "orderacceptedmsg.h"

#include <utilities.h>

#include <stdexcept>

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

OrderAccepted::OrderAccepted(Types::OrderIdType orderId)
    : OrderAccepted(std::move(GlobalUtilities::getLogger()), orderId)
{}
OrderAccepted::OrderAccepted(std::shared_ptr<AbstractLogger> logger,
                             Types::OrderIdType orderId)
    : _orderId(orderId)
{
    if(_orderId <= 0)
    {
        LOG_TRACE(logger,
                  QString("Invalid argument: orderId(%1) <= 0")
                  .arg(_orderId.value));

        throw std::invalid_argument("orderId <= 0.");
    }
}

Types::Message::MessageLengthType OrderAccepted::length() const
{
    return Response::length() + sizeof(_orderId);
}

Types::Message::MessageType OrderAccepted::type() const
{
    return Types::Message::MessageType::RESPONSE_ORDER_ACCEPTED;
}

void OrderAccepted::send(QIODevice* connection)
{
    // Domyślnie BigEndian.
    QDataStream out(connection);

    sendHeader(out);
    out << _orderId;
}



}
}
