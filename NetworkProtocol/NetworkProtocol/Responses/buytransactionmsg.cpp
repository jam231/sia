#include "buytransactionmsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

BuyTransaction::BuyTransaction(Types::OrderIdType orderId, Types::AmountType amount)
{
    m_orderId = orderId;
    m_amount = amount;
}

void BuyTransaction::send(QIODevice *connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);
    out  << m_orderId
         << m_amount;
}

Types::Message::MessageType BuyTransaction::type() const
{
    return Types::Message::MessageType::RESPONSE_BUY_TRANSACTION;
}

Types::Message::MessageLengthType BuyTransaction::length() const
{
    return sizeof(Types::Message::MessageType) +
           sizeof(m_orderId) + sizeof(m_amount);
}

const QString BuyTransaction::getMessageName()
{
    return Response::getMessageName() + " :: BuyTransaction";
}

}
}
