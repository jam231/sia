#include "buytransactionmsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

BuyTransaction::BuyTransaction(Types::OrderIdType orderId, Types::AmountType amount)
{
    _orderId = orderId;
    _amount = amount;
}

void BuyTransaction::send(QIODevice *connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);
    out  << _orderId
         << _amount;
}

Types::Message::MessageType BuyTransaction::type() const
{
    return Types::Message::MessageType::RESPONSE_BUY_TRANSACTION;
}

Types::Message::MessageLengthType BuyTransaction::length() const
{
    return sizeof(Types::Message::MessageType) +
           sizeof(_orderId) + sizeof(_amount);
}


}
}
