#include "DataTransferObjects/bestorder.h"

#include <stdexcept>
#include <utilities.h>

namespace NetworkProtocol
{
namespace DTO
{

BestOrder::BestOrder(Types::Order::OrderType orderType, Types::StockIdType stockId,
                     Types::AmountType amount, Types::PriceType price)
    : m_orderType(orderType), m_stockId(stockId), m_amount(amount), m_price(price)
{
    if(stockId <= 0 || m_amount <= 0 || m_price <= 0)
    {
        LOG_TRACE(QString("stockId(%1) <= 0 || m_amount(%2) <= 0 || m_price(%3) <= 0 == false")
                  .arg(m_stockId.value).arg(m_amount.value).arg(m_price.value));
        throw std::invalid_argument("One of stockId, amount, price is <= 0.");

    }
}

Types::Message::MessageLengthType BestOrder::lengthSerialized() const
{
    return sizeof(m_orderType) + sizeof(m_stockId) + sizeof(m_amount) + sizeof(m_price);
}

Types::Order::OrderType BestOrder::getOrderType() const
{
    return m_orderType;
}

Types::StockIdType BestOrder::getStockId() const
{
    return m_stockId;
}

Types::AmountType BestOrder::getAmount() const
{
    return m_amount;
}

Types::PriceType BestOrder::getPrice() const
{
    return m_price;
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
