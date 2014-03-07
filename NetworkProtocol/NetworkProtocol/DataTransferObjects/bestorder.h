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
    BestOrder(Types::Order::OrderType, Types::StockIdType,
              Types::AmountType, Types::PriceType);
    Types::AmountType getAmount() const;
    Types::PriceType getPrice() const;
    Types::Order::OrderType getOrderType() const;
    Types::StockIdType getStockId() const;

    Types::Message::MessageLengthType lengthSerialized() const;

    friend QDataStream &operator<<(QDataStream& stream, const BestOrder& bestOrder);

    static BestOrder fromStream(QDataStream&);
};


/*
 *  Serialization format:
 *  <order_type : 1><stock_id : 4><amount : 4><price : 4>
 *
 */
NETWORKPROTOCOLSHARED_EXPORT QDataStream &operator<<(QDataStream& stream, const BestOrder& bestOrder);

}
}
#endif // BESTORDER_H
