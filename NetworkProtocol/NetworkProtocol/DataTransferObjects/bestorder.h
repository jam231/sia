#ifndef BESTORDER_H
#define BESTORDER_H

#include <QDataStream>

#include "networkprotocol_global.h"

#include "types.h"

namespace NetworkProtocol
{
namespace DTO
{

class NETWORKPROTOCOLSHARED_EXPORT BestOrder
{
private:
    Types::Order::OrderType m_orderType;
    Types::StockIdType m_stockId;
    Types::AmountType m_amount;
    Types::PriceType m_price;

public:
    BestOrder(Types::Order::OrderType, Types::StockIdType, Types::AmountType, Types::PriceType);

    Types::Order::OrderType getOrderType() const;
    Types::StockIdType getStockId() const;
    Types::AmountType getAmount() const;
    Types::PriceType getPrice() const;

    Types::MessageLengthType lengthInBytes() const;

    friend QDataStream &operator<<(QDataStream& stream, const BestOrder& bestOrder);
    friend QDataStream &operator>>(QDataStream& stream, BestOrder& bestOrder);
};

}
}
#endif // BESTORDER_H
