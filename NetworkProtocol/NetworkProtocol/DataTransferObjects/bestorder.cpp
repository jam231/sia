#include "DataTransferObjects/bestorder.h"

#include <stdexcept>


namespace NetworkProtocol
{
namespace DTO
{
BestOrder::BestOrder(Types::Order::OrderType orderType, Types::StockIdType stockId,
                     Types::AmountType amount, Types::PriceType price)
    : BestOrder(std::move(GlobalUtilities::getLogger()), orderType, stockId, amount, price)
{
}
BestOrder::BestOrder(std::shared_ptr<AbstractLogger> logger,
                     Types::Order::OrderType orderType, Types::StockIdType stockId,
                     Types::AmountType amount, Types::PriceType price)
    : _orderType(orderType), _stockId(stockId), _amount(amount), _price(price)
{
    if(_stockId <= 0 || _amount <= 0 || _price <= 0)
    {
        LOG_TRACE(logger,
                  QString("Invalid arguments: stockId(%1) <= 0 || amount(%2) <= 0 || price(%3) <= 0.")
                  .arg(_stockId.value).arg(_amount.value).arg(_price.value));
        throw std::invalid_argument("One of stock id, amount, price is <= 0.");
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
    return fromStream(std::move(GlobalUtilities::getLogger()), stream);
}

BestOrder BestOrder::fromStream(std::shared_ptr<AbstractLogger> logger,
                                QDataStream& stream)
{
    Types::Order::OrderType order_type;
    Types::StockIdType stock_id;
    Types::AmountType amount;
    Types::PriceType price;

    if(sizeof(stock_id) + sizeof(order_type) +
         sizeof(amount) + sizeof(price) != stream.device()->bytesAvailable())
    {
        LOG_TRACE(logger,
                  QString("Not enough bytes in the stream to read"\
                          " BestOrder. Should be >%1 is %2")
                  .arg(sizeof(stock_id) + sizeof(order_type) +
                       sizeof(amount) + sizeof(price))
                  .arg(stream.device()->bytesAvailable()));

        throw std::invalid_argument("Not enough bytes in the stream to read"\
                                    " BestOrder.");
    }

    stream >> order_type >> stock_id >> amount >> price;

    return BestOrder(std::move(logger), order_type, stock_id, amount, price);
}

}
}
