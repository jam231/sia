#ifndef BESTORDER_H
#define BESTORDER_H

#include <QDataStream>

#include "networkprotocol_global.h"

#include "types.h"

namespace NetworkProtocol
{
namespace DTO
{

class BestOrder
{
private:
    Types::OrderType m_orderType;
    Types::StockIdType m_stockId;
    Types::AmountType m_amount;
    Types::PriceType m_price;

public:
    BestOrder(Types::OrderType, Types::StockIdType, Types::AmountType, Types::PriceType);

    static const qint16 lengthInBytes = sizeof(m_orderType) + sizeof(m_stockId) +
                                        sizeof(m_amount) + sizeof(m_price);

    Types::OrderType getOrderType() const;
    Types::StockIdType getStockId() const;
    Types::AmountType getAmount() const;
    Types::PriceType getPrice() const;

    friend QDataStream &operator<<(QDataStream& stream, const BestOrder& bestOrder);
    friend QDataStream &operator>>(QDataStream& stream, BestOrder& bestOrder);
};

}
}
#endif // BESTORDER_H
