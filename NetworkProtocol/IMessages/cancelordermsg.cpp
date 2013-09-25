#include "cancelordermsg.h"

#include <QDebug>

namespace NetworkProtocol {

CancelOrderMsg::CancelOrderMsg(QDataStream &in)
{
    // Domyślnie BigEndian
    if(in.device()->bytesAvailable() != sizeof(m_orderId))
        throw InvalidDataInMsg();

    in >> m_orderId;
}

IOMessage::MessageType CancelOrderMsg::type() const
{
    return CANCEL_ORDER_REQ;
}

qint32 CancelOrderMsg::getOrderId() const
{
    return m_orderId;
}

}
