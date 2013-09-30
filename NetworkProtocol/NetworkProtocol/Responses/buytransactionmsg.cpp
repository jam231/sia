#include "buytransactionmsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

BuyTransactionMsg::BuyTransactionMsg(Types::OrderIdType orderId, Types::AmountType amount)
{
    m_orderId = orderId;
    m_amount = amount;
}

void BuyTransactionMsg::send(QIODevice *connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);
    out  << m_orderId
         << m_amount;
}

Message::MessageType BuyTransactionMsg::type() const
{
    return BUY_TRANSACTION;
}

Types::MessageLengthType BuyTransactionMsg::length() const
{
    return sizeof(MessageType) + sizeof(m_orderId) + sizeof(m_amount);
}

}
}
