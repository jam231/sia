#include "orderidresp.h"

namespace NetworkProtocol
{
namespace Responses
{
OrderIdResp::OrderIdResp(qint32 orderId) : m_orderId(orderId)
{
}

qint16 OrderIdResp::length() const
{
    return sizeof(MessageType) + sizeof(m_orderId);
}

Message::MessageType OrderIdResp::type() const
{
    return ORDER_ID_RESP;
}

void OrderIdResp::send(QIODevice* connection)
{
    // Domyślnie BigEndian.
    QDataStream out(connection);

    sendHeader(out);
    out << static_cast<qint32>(m_orderId);
}
}
}
