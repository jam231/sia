#include "selltransactionmsg.h"

#include <QDataStream>

SellTransactionMsg::SellTransactionMsg(qint32 orderId, qint32 amount)
{
    m_orderId = orderId;
    m_amount = amount;
}

void SellTransactionMsg::send(QIODevice *connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    out << static_cast<qint16>(sizeof(qint8) + 2 * sizeof(qint32))
        << static_cast<qint8>(type())
        << static_cast<qint32>(m_orderId)
        << static_cast<qint32>(m_amount);
}
IOMessage::MessageType SellTransactionMsg::type() const
{
    return SELL_TRANSACTION;
}

qint32 SellTransactionMsg::length() const
{
    return -1;
}
