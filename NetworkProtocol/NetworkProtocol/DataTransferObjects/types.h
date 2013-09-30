#ifndef TYPES_H
#define TYPES_H

#include "networkprotocol_global.h"

namespace NetworkProtocol
{
namespace DTO
{
namespace Types
{
    // Ids
    typedef qint32 OrderIdType;
    typedef qint32 StockIdType;
    typedef qint32 UserIdType;
    typedef qint32 CompanyIdType;

    typedef qint32 AmountType;
    typedef qint32 PriceType;

    /*
     * Niestety kiedy chcemy mieć typy ładnie zdefiniowane w jednymi miejscu
     * a jednoczesnie chcemy serializowac enumy, musimy zrobic coś takiego - tzn. nic
     * lepszego nie przychodzi mi do glowy.
     *                              -- jam231
     */
    // Enum types
    typedef qint8 MessageTypeType;
    typedef qint8 OrderTypeType;


    typedef qint16 MessageLengthType;

    enum OrderType : OrderTypeType
    {
        UNDEFINED = 0,
        BUY,
        SELL
    };

    OrderType toOrderType(OrderTypeType type);
}
}
}
#endif // TYPES_H
