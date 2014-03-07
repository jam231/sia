#include "lasttransactionmsg.h"

#include <stdexcept>
#include <utilities.h>

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO::Types;

LastTransaction::LastTransaction(StockIdType stockId, AmountType amount,
                                 PriceType price, QString dateTime)
    : LastTransaction(stockId, DTO::LastTransaction(amount, price, dateTime))
{
}

LastTransaction::LastTransaction(StockIdType stockId,
                                 DTO::LastTransaction last_transaction)
    : _stockId(stockId), _lastTransaction(last_transaction)
{
    if(_stockId <= 0)
    {
        GLOBAL_LOG_TRACE(QString("Invalid arguments: stockId(%1) <= 0")
                         .arg(_stockId.value));
        throw std::invalid_argument("stockId <= 0.");
    }
}

Message::MessageLengthType LastTransaction::length() const
{
    return Response::length() +
           sizeof(_stockId) + _lastTransaction.lengthSerialized();
}

void LastTransaction::send(QIODevice *connection)
{
    // Domyślnie BigEndian.
    QDataStream out(connection);
    sendHeader(out);
    out << _stockId << _lastTransaction;
}

Message::MessageType LastTransaction::type() const
{
    return DTO::Types::Message::MessageType::RESPONSE_LAST_TRANSACTION;
}


}
}
