#include "buytransactionmsg.h"
#include <QDataStream>

BuyTransactionMsg::BuyTransactionMsg(qint32 orderId, qint32 amount)
{
    m_orderId = orderId;
    m_amount = amount;
}

void BuyTransactionMsg::send(QIODevice *connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);
    out  << static_cast<qint32>(m_orderId)
        << static_cast<qint32>(m_amount);
}
IOMessage::MessageType BuyTransactionMsg::type() const
{
    return BUY_TRANSACTION;
}
qint16 BuyTransactionMsg::length() const
{
    return sizeof(MessageType) + 2 * sizeof(qint32);
}
