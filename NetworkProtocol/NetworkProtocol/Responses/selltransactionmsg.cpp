#include "selltransactionmsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

SellTransaction::SellTransaction(Types::OrderIdType orderId,
                                Types::AmountType amount)
{
    m_orderId = orderId;
    m_amount = amount;
}

void SellTransaction::send(QIODevice *connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);
    out << m_orderId
        << m_amount;
}

Types::Message::MessageType SellTransaction::type() const
{
    return Types::Message::MessageType::RESPONSE_SELL_TRANSACTION;
}

Types::Message::MessageLengthType SellTransaction::length() const
{
    return sizeof(Types::Message::MessageType) +
            sizeof(m_orderId) + sizeof(m_amount);
}

const QString SellTransaction::getMessageName()
{
    return Response::getMessageName() + " :: SellTransaction";
}

}
}
