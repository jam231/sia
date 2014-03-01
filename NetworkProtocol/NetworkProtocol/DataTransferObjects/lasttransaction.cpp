#include "lasttransaction.h"

#include <stdexcept>
#include <utilities.h>

namespace NetworkProtocol
{
namespace DTO
{

QString LastTransaction::getDateTime() const
{
    return _dateTime;
}

Types::AmountType LastTransaction::getAmount() const
{
    return _amount;
}

Types::PriceType LastTransaction::getPrice() const
{
    return _price;
}

Types::Message::MessageLengthType LastTransaction::lengthSerialized() const
{
    return sizeof(Types::Message::MessageLengthType) + _dateTime.toUtf8().size() +
           sizeof(_amount) + sizeof(_price);
}

LastTransaction::LastTransaction(QString dateTime, Types::AmountType amount,
                                 Types::PriceType price)
    : _dateTime(dateTime), _amount(amount), _price(price)
{
    if(_amount <= 0 || _price <= 0)
    {
        LOG_TRACE(QString("amount(%1) <= 0 || price(%2) <= 0 == false")
                  .arg(_amount.value).arg(_price.value));
        throw std::invalid_argument("One of amount, price is <= 0.");

    }
}


QDataStream &operator<<(QDataStream& stream, const LastTransaction& lastTransaction)
{
    QByteArray dateTime = lastTransaction._dateTime.toUtf8();

    stream << lastTransaction._amount
           << lastTransaction._price
           << static_cast<Types::Message::MessageLengthType>(lastTransaction._dateTime.toUtf8().size());
    stream.device()->write(dateTime);
    return stream;
}

}
}

