#include "DataTransferObjects/bestorder.h"

namespace NetworkProtocol
{
namespace DTO
{

BestOrder::BestOrder(Types::Order::OrderType orderType, Types::StockIdType stockId,
                     Types::AmountType amount, Types::PriceType price)
    : m_orderType(orderType), m_stockId(stockId), m_amount(amount), m_price(price)
{
}

Types::MessageLengthType BestOrder::lengthInBytes() const
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

QDataStream &operator>>(QDataStream& stream, BestOrder& bestOrder)
{

    stream >> bestOrder.m_orderType
           >> bestOrder.m_stockId
           >> bestOrder.m_amount
           >> bestOrder.m_price;
    return stream;
}

}
}
