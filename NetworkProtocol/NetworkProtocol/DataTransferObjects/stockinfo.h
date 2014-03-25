#ifndef STOCKINFO_H
#define STOCKINFO_H

#include "networkprotocol_global.h"
#include "bestorder.h"
#include "lasttransaction.h"
#include "types.h"

#include <utilities.h>

#include <memory>


namespace NetworkProtocol
{
namespace DTO
{

class NETWORKPROTOCOLSHARED_EXPORT StockInfo
{
protected:
    DTO::Types::StockIdType _stockId;
    std::shared_ptr<DTO::BestOrder> _bestBuyOrder;
    std::shared_ptr<DTO::BestOrder> _bestSellOrder;
    std::shared_ptr<DTO::LastTransaction> _lastTransaction;


    Types::Message::MessageLengthType getSerializedLength(DTO::BestOrder*) const;
    Types::Message::MessageLengthType getSerializedLength(DTO::LastTransaction*) const;
public:
    StockInfo(std::shared_ptr<AbstractLogger>, Types::StockIdType stockId,
              std::shared_ptr<BestOrder> bestBuyOrder = nullptr,
              std::shared_ptr<BestOrder> bestSellOrder = nullptr,
              std::shared_ptr<LastTransaction> lastTransaction = nullptr);

    StockInfo(Types::StockIdType stockId,
              std::shared_ptr<BestOrder> bestBuyOrder = nullptr,
              std::shared_ptr<BestOrder> bestSellOrder = nullptr,
              std::shared_ptr<LastTransaction> lastTransaction = nullptr)
        : StockInfo(std::move(GlobalUtilities::getLogger()), stockId,
                    std::move(bestBuyOrder), std::move(bestSellOrder),
                    std::move(lastTransaction))
    {
    }

    Types::Message::MessageLengthType lengthSerialized() const;

    Types::StockIdType getStockId() const;
    std::shared_ptr<BestOrder> getBestBuyOrder() const;
    std::shared_ptr<BestOrder> getBestSellOrder() const;
    std::shared_ptr<LastTransaction> getLastTransaction() const;
};

// NETWORKPROTOCOLSHARED_EXPORT QDataStream& operator<<(QDataStream&, const StockInfo&);

}
}
#endif // STOCKINFO_H
