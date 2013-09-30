#include "DataTransferObjects/bestorder.h"

namespace NetworkProtocol
{
namespace DTO
{

BestOrder::BestOrder(Types::OrderType orderType, Types::StockIdType stockId,
                     Types::AmountType amount, Types::PriceType price)
    : m_orderType(orderType), m_stockId(stockId), m_amount(amount), m_price(price)
{
}

Types::OrderType BestOrder::getOrderType() const
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
    Types::OrderTypeType orderType;
    stream >> orderType
           >> bestOrder.m_stockId
           >> bestOrder.m_amount
           >> bestOrder.m_price;
    bestOrder.m_orderType = Types::toOrderType(orderType);
    return stream;
}

}
}
