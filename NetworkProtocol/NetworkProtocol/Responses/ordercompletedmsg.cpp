#include "ordercompletedmsg.h"

#include <utilities.h>
#include <stdexcept>

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

OrderCompleted::OrderCompleted(Types::OrderIdType orderId)
    : OrderCompleted(std::move(GlobalUtilities::getLogger()), orderId)
{}

OrderCompleted::OrderCompleted(std::shared_ptr<AbstractLogger> logger,
                               Types::OrderIdType orderId)
    : _orderId(orderId)
{

    if(_orderId <= 0)
    {
        LOG_TRACE(logger,
                  QString("Invalid arguments: orderId(%1) <= 0.")
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

Types::OrderIdType OrderCompleted::getOrderId() const
{
    return _orderId;
}

}
}
