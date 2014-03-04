#include "selltransactionmsg.h"

#include <utilities.h>
#include <stdexcept>

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

SellTransaction::SellTransaction(Types::OrderIdType orderId,
                                 Types::AmountType amount)
    : _orderId(orderId), _amount(amount)
{
    if(_orderId <= 0 || _amount <= 0)
    {
        GLOBAL_LOG_TRACE(QString("Invalid arguments: orderId(%1) <= 0 || amount(%3) <= 0 ")
                  .arg(_orderId.value).arg(_amount.value));
        throw std::invalid_argument("One of orderId, amount is <= 0.");
    }
}

void SellTransaction::send(QIODevice *connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);
    out << _orderId << _amount;
}

Types::Message::MessageType SellTransaction::type() const
{
    return Types::Message::MessageType::RESPONSE_SELL_TRANSACTION;
}

Types::Message::MessageLengthType SellTransaction::length() const
{
    return sizeof(_orderId) + sizeof(_amount);
}


}
}
