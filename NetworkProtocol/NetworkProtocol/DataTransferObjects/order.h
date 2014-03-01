#ifndef ORDER_H
#define ORDER_H

#include <QDataStream>

#include "networkprotocol_global.h"

#include "types.h"

namespace NetworkProtocol
{
namespace DTO
{

class NETWORKPROTOCOLSHARED_EXPORT Order
{

private:
    Types::OrderIdType _orderId;
    Types::Order::OrderType _orderType;
    Types::StockIdType _stockId;
    Types::AmountType _amount;
    Types::PriceType _price;
public:
    Order(Types::OrderIdType, Types::Order::OrderType, Types::StockIdType,
          Types::AmountType, Types::PriceType);

    Types::OrderIdType getOrderId() const;
    Types::Order::OrderType getOrderType() const;
    Types::StockIdType getStockId() const;
    Types::AmountType getAmount() const;
    Types::PriceType getPrice() const;

    Types::Message::MessageLengthType lengthSerialized() const;

    friend QDataStream &operator<<(QDataStream& stream, const Order& order);

    static Order fromStream(QDataStream&);
};

/*
 *  Serialization format:
 *  <order_id : 4><order_type : 1><stock_id : 4><amount : 4><price : 4>
 *
 */
NETWORKPROTOCOLSHARED_EXPORT QDataStream &operator<<(QDataStream& stream, const Order&);

}
}

#endif // ORDER_H
