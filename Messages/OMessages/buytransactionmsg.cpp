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

    out << static_cast<qint16>(sizeof(qint8) + 2 * sizeof(qint32))
        << static_cast<qint8>(type())
        << static_cast<qint32>(m_orderId)
        << static_cast<qint32>(m_amount);
}
IOMessage::MessageType BuyTransactionMsg::type() const
{
    return BUY_TRANSACTION;
}
qint16 BuyTransactionMsg::length() const
{
    return -1;
}
