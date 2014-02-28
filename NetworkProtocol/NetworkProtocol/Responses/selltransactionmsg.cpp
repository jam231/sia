#include "selltransactionmsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

SellTransaction::SellTransaction(Types::OrderIdType orderId,
                                Types::AmountType amount)
{
    _orderId = orderId;
    _amount = amount;
}

void SellTransaction::send(QIODevice *connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);
    out << _orderId
        << _amount;
}

Types::Message::MessageType SellTransaction::type() const
{
    return Types::Message::MessageType::RESPONSE_SELL_TRANSACTION;
}

Types::Message::MessageLengthType SellTransaction::length() const
{
    return sizeof(Types::Message::MessageType) +
            sizeof(_orderId) + sizeof(_amount);
}


}
}
