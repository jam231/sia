#include "cancelordermsg.h"

#include <QDebug>

namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

CancelOrder::CancelOrder(QDataStream &in) : Request(in)
{
    in >> _orderId;
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
    return sizeof(Types::Message::MessageType) +
           sizeof(_orderId);
}


}
}
