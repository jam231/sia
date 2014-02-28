#ifndef BESTORDER_H
#define BESTORDER_H

#include <QDataStream>

#include "networkprotocol_global.h"

#include "types.h"

namespace NetworkProtocol
{
namespace DTO
{

class NETWORKPROTOCOLSHARED_EXPORT BestOrder
{
protected:
    Types::AmountType _amount;
    Types::PriceType _price;
    Types::Order::OrderType _orderType;
    Types::StockIdType _stockId;
public:
    BestOrder(Types::Order::OrderType, Types::StockIdType, Types::AmountType, Types::PriceType);
    Types::AmountType getAmount() const;
    Types::PriceType getPrice() const;
    Types::Order::OrderType getOrderType() const;
    Types::StockIdType getStockId() const;

    Types::Message::MessageLengthType lengthSerialized() const;

    friend QDataStream &operator<<(QDataStream& stream, const BestOrder& bestOrder);

    static BestOrder fromStream(QDataStream&);
};
/*
 *  It serializes contents of BestOrder object, however it does NOT insert at the begining
 *  length !
 */
NETWORKPROTOCOLSHARED_EXPORT QDataStream &operator<<(QDataStream& stream, const BestOrder& bestOrder);

}
}
#endif // BESTORDER_H
