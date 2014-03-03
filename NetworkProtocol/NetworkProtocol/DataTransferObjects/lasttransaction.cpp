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
        LOG_TRACE(QString("Invalid arguments: amount(%1) <= 0 || price(%2) <= 0")
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

LastTransaction LastTransaction::fromStream(QDataStream& stream)
{
    Types::Message::MessageLengthType date_length;
    Types::AmountType amount;
    Types::PriceType price;

    if(stream.device()->bytesAvailable() <= sizeof(date_length))
    {
        LOG_TRACE(QString("Wrong number of bytes in the stream to read"\
                          " LastTransaction. Should be >%1 is %2")
                  .arg(sizeof(date_length))
                  .arg(stream.device()->bytesAvailable()));
        throw std::runtime_error("Not enough bytes in the stream to read LastTransaction.");
    }
    stream >> date_length;

    if(date_length + sizeof(amount) + sizeof(price) > stream.device()->bytesAvailable())
    {
        LOG_TRACE(QString("Wrong number of bytes in the stream to read"\
                          " LastTransaction. Should be %1 is %2")
                  .arg(date_length + sizeof(amount) + sizeof(price))
                  .arg(stream.device()->bytesAvailable()));
        throw std::runtime_error("Not enough bytes in the stream to read LastTransaction.");
    }
    QByteArray buffer;
    buffer.resize(date_length);
    stream.readRawData(buffer.data(), date_length);
    stream >> amount >> price;
    return LastTransaction(QString::fromUtf8(buffer), amount, price);
}

}
}

