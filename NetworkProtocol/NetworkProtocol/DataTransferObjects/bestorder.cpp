#include "DataTransferObjects/bestorder.h"

#include <stdexcept>
#include <utilities.h>

namespace NetworkProtocol
{
namespace DTO
{

BestOrder::BestOrder(Types::Order::OrderType orderType, Types::StockIdType stockId,
                     Types::AmountType amount, Types::PriceType price)
    : _orderType(orderType), _stockId(stockId), _amount(amount), _price(price)
{
    if(stockId <= 0 || _amount <= 0 || _price <= 0)
    {
        LOG_TRACE(QString("stockId(%1) <= 0 || _amount(%2) <= 0 || _price(%3) <= 0 == false")
                  .arg(_stockId.value).arg(_amount.value).arg(_price.value));
        throw std::invalid_argument("One of stockId, amount, price is <= 0.");

    }
}

Types::Message::MessageLengthType BestOrder::lengthSerialized() const
{
    return sizeof(_orderType) + sizeof(_stockId) + sizeof(_amount) + sizeof(_price);
}

Types::Order::OrderType BestOrder::getOrderType() const
{
    return _orderType;
}

Types::StockIdType BestOrder::getStockId() const
{
    return _stockId;
}
Types::AmountType BestOrder::getAmount() const
{
    return _amount;
}

Types::PriceType BestOrder::getPrice() const
{
    return _price;
}


QDataStream &operator<<(QDataStream& stream, const BestOrder& bestOrder)
{
    stream << bestOrder.getOrderType()
           << bestOrder.getStockId()
           << bestOrder.getAmount()
           << bestOrder.getPrice();
    return stream;
}


BestOrder BestOrder::fromStream(QDataStream& stream)
{
    Types::Order::OrderType order_type;
    Types::StockIdType stock_id;
    Types::AmountType amount;
    Types::PriceType price;
    stream >> order_type >> stock_id >> amount >> price;
    return BestOrder(order_type, stock_id, amount, price);
}

}
}
