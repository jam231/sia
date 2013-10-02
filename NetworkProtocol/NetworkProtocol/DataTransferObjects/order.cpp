#include "order.h"

namespace NetworkProtocol
{
namespace DTO
{

Order::Order(Types::OrderIdType orderId, Types::Order::OrderType orderType, Types::StockIdType stockId,
             Types::AmountType amount, Types::PriceType price)

    : m_orderId(orderId), m_orderType(orderType),
      m_stockId(stockId), m_amount(amount), m_price(price)
{
}

Types::OrderIdType Order::getOrderId() const
{
    return m_orderId;
}
Types::Order::OrderType Order::getOrderType() const
{
    return m_orderType;
}
Types::StockIdType Order::getStockId() const
{
    return m_stockId;
}
Types::AmountType Order::getAmount() const
{
    return m_amount;
}
Types::PriceType Order::getPrice() const
{
    return m_price;
}

Types::MessageLengthType Order::lengthInBytes() const
{
    return sizeof(m_orderId) + sizeof(m_orderType) +
           sizeof(m_stockId) + sizeof(m_amount) + sizeof(m_price);
}

QDataStream &operator<<(QDataStream& stream, const Order& order)
{
    stream << order.m_orderId
           << order.m_orderType
           << order.m_stockId
           << order.m_amount
           << order.m_price;

    return stream;
}

QDataStream &operator>>(QDataStream& stream, Order& order)
{
    stream >> order.m_orderId
           >> order.m_orderType
           >> order.m_stockId
           >> order.m_amount
           >> order.m_price;

    return stream;
}

}
}
