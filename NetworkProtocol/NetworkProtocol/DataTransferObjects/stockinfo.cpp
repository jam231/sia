#include "stockinfo.h"

namespace NetworkProtocol
{
namespace DTO
{

using namespace std;
using namespace Types;

StockInfo::StockInfo(std::shared_ptr<AbstractLogger>,
                     Types::StockIdType stockId,
                     std::shared_ptr<BestOrder> bestBuyOrder,
                     std::shared_ptr<BestOrder> bestSellOrder,
                     std::shared_ptr<LastTransaction> lastTransaction)
    : _stockId(stockId), _bestBuyOrder(move(bestBuyOrder)),
      _bestSellOrder(move(bestSellOrder)), _lastTransaction(move(lastTransaction))
{
}


Message::MessageLengthType StockInfo::getSerializedLength(
        DTO::BestOrder* best_order) const
{
    if(best_order == nullptr)
    {
        return sizeof(qint32);
    }
    else
    {
        return sizeof(AmountType) + sizeof(PriceType);
    }
}

Message::MessageLengthType StockInfo::getSerializedLength(
        DTO::LastTransaction* last_transaction) const
{
    if(last_transaction == nullptr)
    {
        return sizeof(qint32);
    }
    else
    {
        return  last_transaction->lengthSerialized();
    }
}

Types::Message::MessageLengthType StockInfo::lengthSerialized() const
{
    return sizeof(_stockId) + getSerializedLength(_bestBuyOrder.get()) +
            getSerializedLength(_bestSellOrder.get()) +
            getSerializedLength(_lastTransaction.get());
}

std::shared_ptr<LastTransaction> StockInfo::getLastTransaction() const
{
    return  _lastTransaction;
}

std::shared_ptr<BestOrder> StockInfo::getBestBuyOrder() const
{
    return  _bestBuyOrder;
}

std::shared_ptr<BestOrder> StockInfo::getBestSellOrder() const
{
    return  _bestSellOrder;
}

StockIdType StockInfo::getStockId() const
{
    return _stockId;
}

}
}
