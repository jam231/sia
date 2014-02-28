#ifndef LASTTRANSACTION_H
#define LASTTRANSACTION_H

#include <QDataStream>

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
    LastTransaction(QString dateTime, DTO::Types::AmountType, DTO::Types::PriceType);

    QString getDateTime() const;
    DTO::Types::AmountType getAmount() const;
    DTO::Types::PriceType getPrice() const;

    DTO::Types::Message::MessageLengthType lengthSerialized() const;

    friend QDataStream &operator<<(QDataStream& stream, const LastTransaction&);

};
/*
 *  It serializes contents of LastTransaction object, however it does NOT insert at the begging
 *  length !
 */
NETWORKPROTOCOLSHARED_EXPORT QDataStream& operator<<(QDataStream&, const LastTransaction&);

}
}
#endif // LASTTRANSACTION_H
