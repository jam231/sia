#include "cancelordermsg.h"

#include <QDebug>

namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

CancelOrder::CancelOrder(QDataStream &in) : Request(in)
{
    in >> m_orderId;
}

Types::Message::MessageType CancelOrder::type() const
{
    return Types::Message::MessageType::REQUEST_CANCEL_ORDER;
}

Types::OrderIdType CancelOrder::getOrderId() const
{
    return m_orderId;
}

Types::MessageLengthType CancelOrder::length() const
{
    return sizeof(Types::Message::MessageType) +
           sizeof(m_orderId);
}

const QString CancelOrder::getMessageName()
{
    return Request::getMessageName() + " :: CancelOrder";
}


}
}
