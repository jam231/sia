#include "order.h"

namespace NetworkProtocol
{
namespace DTO
{

Order::Order(Types::OrderIdType orderId, Types::OrderType orderType, Types::StockIdType stockId,
             Types::AmountType amount, Types::PriceType price)

    : m_orderId(orderId), m_orderType(orderType),
      m_stockId(stockId), m_amount(amount), m_price(price)
{
}
/*
Order::Order(Types::OrderType orderType, Types::StockIdType stockId,
             Types::AmountType amount, Types::PriceType price)
    : Order(-1, orderType, stockId, amount, price)
{
}

Order::Order()
    : m_orderId(-1), m_transactionType(UNDEFINED),
      m_stockId(-1), m_amount(-1), m_price(-1)
{
}
*/
Types::OrderIdType Order::getOrderId() const
{
    return m_orderId;
}
Types::OrderType Order::getOrderType() const
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

QDataStream &operator<<(QDataStream& stream, const Order& order)
{
    stream << static_cast<Types::OrderIdType>(order.getOrderId())
           << static_cast<Types::OrderType>(order.getOrderType())
           << static_cast<Types::StockIdType>(order.getStockId())
           << static_cast<Types::AmountType>(order.getAmount())
           << static_cast<Types::PriceType >(order.getPrice());

    return stream;
}

QDataStream &operator>>(QDataStream& stream, Order& order)
{
    Types::OrderTypeType orderType;
    stream >> order.m_orderId
           >> orderType
           >> order.m_stockId
           >> order.m_amount
           >> order.m_price;
    order.m_orderType = Types::toOrderType(orderType);
    return stream;
}

}
}
