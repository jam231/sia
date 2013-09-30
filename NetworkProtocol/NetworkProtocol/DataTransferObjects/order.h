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
    Types::OrderIdType m_orderId;
    Types::OrderType m_orderType;
    Types::StockIdType m_stockId;
    Types::AmountType m_amount;
    Types::PriceType m_price;
public:

    static const qint16 lengthInBytes = sizeof(m_orderId) + sizeof(m_orderType) +
                                        sizeof(m_stockId) + sizeof(m_amount) + sizeof(m_price);

    Order(Types::OrderIdType, Types::OrderType, Types::StockIdType, Types::AmountType, Types::PriceType);
    //Order(Types::OrderType, Types::StockIdType, Types::AmountType, Types::PriceType);
    //Order();

    Types::OrderIdType getOrderId() const;
    Types::OrderType getOrderType() const;
    Types::StockIdType getStockId() const;
    Types::AmountType getAmount() const;
    Types::PriceType getPrice() const;

    friend QDataStream &operator<<(QDataStream& stream, const Order& order);
    friend QDataStream &operator>>(QDataStream& stream, Order& order);
};
}
}

#endif // ORDER_H
