#include "orderidmsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

OrderId::OrderId(Types::OrderIdType orderId) : m_orderId(orderId)
{
}

Types::Message::MessageLengthType OrderId::length() const
{
    return sizeof(Types::Message::MessageType) +
           sizeof(m_orderId);
}

Types::Message::MessageType OrderId::type() const
{
    return Types::Message::MessageType::RESPONSE_ORDER_ID;
}

void OrderId::send(QIODevice* connection)
{
    // Domyślnie BigEndian.
    QDataStream out(connection);

    sendHeader(out);
    out << m_orderId;
}



}
}
