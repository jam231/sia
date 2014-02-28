#include "transactionchangemsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

TransactionChange::TransactionChange(Types::StockIdType stockId, Types::AmountType amount,
                                     Types::PriceType price, QString dateTime)
{
    _stockId = stockId;
    _amount = amount;
    _price = price;
    _dateTime = dateTime;
}

Types::Message::MessageLengthType TransactionChange::length() const
{
    return sizeof(Types::Message::MessageType) +
            sizeof(_stockId) + sizeof(_amount) + sizeof(_price) + _dateTime.toUtf8().size();
}

void TransactionChange::send(QIODevice *connection)
{
    // Domyślnie BigEndian.
    QDataStream out(connection);

    QByteArray dateTime_bytes = _dateTime.toUtf8();

    sendHeader(out);
    out << _stockId
        << _amount
        << _price
        << static_cast<Types::Message::MessageLengthType>(dateTime_bytes.size());
    connection->write(dateTime_bytes);
}

Types::Message::MessageType TransactionChange::type() const
{
    return Types::Message::MessageType::RESPONSE_TRANSACTION_CHANGE;
}


}
}
