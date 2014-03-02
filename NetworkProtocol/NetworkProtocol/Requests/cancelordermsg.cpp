#include "cancelordermsg.h"

#include "networkprotocol_utilities.h"

#include <QDebug>

namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

CancelOrder::CancelOrder(QDataStream &serialized_request)
{
    if(serialized_request.device()->bytesAvailable() < sizeof(_orderId))
    {
        LOG_TRACE(QString("Malformed request: Not enough bytes in serialized_request"\
                          " to order id. Is %1 should be >%2.")
                  .arg(serialized_request.device()->bytesAvailable())
                  .arg(sizeof(_orderId)));
        throw MalformedRequest("Not enough bytes in serialized_request to read"\
                               " order id.");
    }

    serialized_request >> _orderId;

    if(_orderId <= 0)
    {
        LOG_TRACE(QString("Invalid order id. orderId(%1) <= 0.")
                  .arg(_orderId.value));
        throw InvalidRequestBody("order id <= 0.");
    }
}

Types::Message::MessageType CancelOrder::type() const
{
    return Types::Message::MessageType::REQUEST_CANCEL_ORDER;
}

Types::OrderIdType CancelOrder::getOrderId() const
{
    return _orderId;
}

Types::Message::MessageLengthType CancelOrder::length() const
{
    return sizeof(_orderId);
}


}
}
