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

Types::Message::MessageType CancelOrderMsg::type() const
{
    return Types::Message::MessageType::CANCEL_ORDER_REQ;
}

Types::OrderIdType CancelOrderMsg::getOrderId() const
{
    return m_orderId;
}

Types::MessageLengthType CancelOrderMsg::length() const
{
    return sizeof(Types::Message::MessageType) +
           sizeof(m_orderId);
}

const QString CancelOrderMsg::getMessageName()
{
    return Request::getMessageName() + " :: CancelOrderMsg";
}


}
}
