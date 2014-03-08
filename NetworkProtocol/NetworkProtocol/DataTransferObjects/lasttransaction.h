#ifndef LASTTRANSACTION_H
#define LASTTRANSACTION_H

#include <QDataStream>

#include <memory>

#include <utilities.h>

#include "networkprotocol_global.h"

#include "types.h"

namespace NetworkProtocol
{
namespace DTO
{

class NETWORKPROTOCOLSHARED_EXPORT LastTransaction
{
    QString _dateTime;
    DTO::Types::AmountType _amount;
    DTO::Types::PriceType _price;

public:
    LastTransaction(std::shared_ptr<AbstractLogger>,
                    DTO::Types::AmountType, DTO::Types::PriceType, QString dateTime);
    LastTransaction(DTO::Types::AmountType, DTO::Types::PriceType, QString dateTime);

    QString getDateTime() const;
    DTO::Types::AmountType getAmount() const;
    DTO::Types::PriceType getPrice() const;

    DTO::Types::Message::MessageLengthType lengthSerialized() const;

    friend QDataStream &operator<<(QDataStream& stream, const LastTransaction&);

    static LastTransaction fromStream(std::shared_ptr<AbstractLogger>, QDataStream&);
    static LastTransaction fromStream(QDataStream&);
};

/*
 *  Serialization format:
 *  <amount : 4><price : 4><data_time_str_length : 2><date_time_str : data_time_str_length>
 *
 */
NETWORKPROTOCOLSHARED_EXPORT QDataStream& operator<<(QDataStream&, const LastTransaction&);

}
}
#endif // LASTTRANSACTION_H
