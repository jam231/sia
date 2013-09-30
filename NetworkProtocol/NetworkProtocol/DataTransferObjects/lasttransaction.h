#ifndef LASTTRANSACTION_H
#define LASTTRANSACTION_H

#include <QDataStream>

#include "networkprotocol_global.h"

#include "types.h"

namespace NetworkProtocol
{
namespace DTO
{

class LastTransaction
{
    QString m_dateTime;
    DTO::Types::AmountType m_amount;
    DTO::Types::PriceType m_price;

public:
    LastTransaction(QString dateTime, DTO::Types::AmountType, DTO::Types::PriceType);
    LastTransaction();

    QString getDateTime() const;
    DTO::Types::AmountType getAmount() const;
    DTO::Types::PriceType getPrice() const;

    DTO::Types::MessageLengthType lengthInBytes() const;
};


}
}
#endif // LASTTRANSACTION_H
