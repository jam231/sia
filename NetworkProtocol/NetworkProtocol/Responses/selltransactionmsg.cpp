#include "selltransactionmsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

SellTransactionMsg::SellTransactionMsg(Types::OrderIdType orderId,
                                       Types::AmountType amount)
{
    m_orderId = orderId;
    m_amount = amount;
}

void SellTransactionMsg::send(QIODevice *connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);
    out << m_orderId
        << m_amount;
}

Types::MessageType SellTransactionMsg::type() const
{
    return Types::MessageType::SELL_TRANSACTION;
}

Types::MessageLengthType SellTransactionMsg::length() const
{
    return sizeof(Types::MessageType) +
            sizeof(m_orderId) + sizeof(m_amount);
}

}
}
