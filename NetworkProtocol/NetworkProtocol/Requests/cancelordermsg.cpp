#include "cancelordermsg.h"

#include <QDebug>

namespace NetworkProtocol
{
namespace Requests
{

CancelOrderMsg::CancelOrderMsg(QDataStream &in)
{
    in >> m_orderId;
}

Message::MessageType CancelOrderMsg::type() const
{
    return CANCEL_ORDER_REQ;
}

qint32 CancelOrderMsg::getOrderId() const
{
    return m_orderId;
}

qint16 CancelOrderMsg::length() const
{
    return sizeof(MessageType) + sizeof(m_orderId);
}

}
}
