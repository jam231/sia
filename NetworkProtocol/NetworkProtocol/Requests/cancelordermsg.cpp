#include "cancelordermsg.h"

#include <QDebug>

namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

CancelOrderMsg::CancelOrderMsg(QDataStream &in)
{
    in >> m_orderId;
}

Types::MessageType CancelOrderMsg::type() const
{
    return Types::MessageType::CANCEL_ORDER_REQ;
}

Types::OrderIdType CancelOrderMsg::getOrderId() const
{
    return m_orderId;
}

Types::MessageLengthType CancelOrderMsg::length() const
{
    return sizeof(Types::MessageType) +
           sizeof(m_orderId);
}

const QString CancelOrderMsg::getMessageName()
{
    return Request::getMessageName() + " :: CancelOrderMsg";
}


}
}
