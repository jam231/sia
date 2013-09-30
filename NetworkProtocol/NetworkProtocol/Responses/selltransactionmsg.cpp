#include "selltransactionmsg.h"

namespace NetworkProtocol
{
namespace Responses
{

SellTransactionMsg::SellTransactionMsg(qint32 orderId, qint32 amount)
{
    m_orderId = orderId;
    m_amount = amount;
}

void SellTransactionMsg::send(QIODevice *connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);
    out << static_cast<qint32>(m_orderId)
        << static_cast<qint32>(m_amount);
}
Message::MessageType SellTransactionMsg::type() const
{
    return SELL_TRANSACTION;
}

qint16 SellTransactionMsg::length() const
{
    return sizeof(MessageType) + 2 * sizeof(qint32);
}

}
}
