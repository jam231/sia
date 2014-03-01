#include "order.h"

#include <stdexcept>
#include <utilities.h>

namespace NetworkProtocol
{
namespace DTO
{

Order::Order(Types::OrderIdType orderId, Types::Order::OrderType orderType,
             Types::StockIdType stockId, Types::AmountType amount,
             Types::PriceType price)

    : _orderId(orderId), _orderType(orderType),
      _stockId(stockId), _amount(amount), _price(price)
{
    if(_orderId <= 0 || _stockId <= 0 || _amount <= 0 || _price <= 0)
    {
        LOG_TRACE(QString("Invalid arguments: orderId(%1) <= 0 || stockId(%2) <= 0 || amount(%3) <= 0 || price(%4) <= 0")
                  .arg(_orderId.value).arg(_stockId.value).arg(_amount.value).arg(_price.value));
        throw std::invalid_argument("One of orderId, stockId, amount, price is <= 0.");
    }
}

Types::OrderIdType Order::getOrderId() const
{
    return _orderId;
}
Types::Order::OrderType Order::getOrderType() const
{
    return _orderType;
}
Types::StockIdType Order::getStockId() const
{
    return _stockId;
}
Types::AmountType Order::getAmount() const
{
    return _amount;
}
Types::PriceType Order::getPrice() const
{
    return _price;
}

Types::Message::MessageLengthType Order::lengthSerialized() const
{
    return sizeof(_orderId) + sizeof(_orderType) +
           sizeof(_stockId) + sizeof(_amount) + sizeof(_price);
}

QDataStream &operator<<(QDataStream& stream, const Order& order)
{
    stream << order._orderId
           << order._orderType
           << order._stockId
           << order._amount
           << order._price;

    return stream;
}
Order Order::fromStream(QDataStream& stream)
{
    Types::OrderIdType order_id;
    Types::Order::OrderType order_type;
    Types::StockIdType stock_id;
    Types::AmountType amount;
    Types::PriceType price;

    stream >> order_id >> order_type
           >> stock_id >> amount >> price;
    return Order(order_id, order_type, stock_id, amount, price);
}

}
}
