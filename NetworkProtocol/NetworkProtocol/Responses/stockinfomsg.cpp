#include "stockinfomsg.h"

#include <memory>

#include <assert.h>

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;
using namespace DTO::Types;

StockInfo::StockInfo (Types::StockIdType stockId,
                      std::shared_ptr<BestOrder> bestBuyOrder,
                      std::shared_ptr<BestOrder> bestSellOrder,
                      std::shared_ptr<LastTransaction> lastTransaction)
    : _stockId(stockId), _bestBuyOrder(bestBuyOrder),
      _bestSellOrder(bestSellOrder), _lastTransaction(lastTransaction)
{
    // Maybe it should instead throw an exception ? I think that the violation below
    // can be caused only by a developer's mistake, therefore when assert fails there is a bug in program
    // that needs to be fixed, so it should fail.
    assert(_bestBuyOrder == nullptr ||
           (_bestBuyOrder->getOrderType() == Order::BUY && _bestBuyOrder->getStockId() == _stockId));
    assert(_bestSellOrder == nullptr ||
           (_bestSellOrder->getOrderType() == Order::BUY && _bestSellOrder->getStockId() == _stockId));

}

void StockInfo::send(BestOrder* bestOrder, QDataStream &out)
{
    if(bestOrder == nullptr)
    {
        out << -1;
    }
    else
    {
        out << bestOrder->getAmount() << bestOrder->getPrice();
    }
}

void StockInfo::send(LastTransaction* lastTransaction, QDataStream& out)
{
    if(lastTransaction == nullptr)
    {
        out << -1;
    }
    else
    {
        QByteArray dateTime = lastTransaction->getDateTime().toUtf8();
        out << lastTransaction->getPrice()
            << static_cast<Types::Message::MessageLengthType>(dateTime.size());
        out.device()->write(dateTime);
    }
}

Message::MessageLengthType StockInfo::getSerializedLength(DTO::BestOrder* best_order) const
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

Message::MessageLengthType StockInfo::getSerializedLength(DTO::LastTransaction* last_transaction) const
{
    if(last_transaction == nullptr)
    {
        return sizeof(qint32);
    }
    else
    {
        return sizeof(Types::Message::MessageLengthType) +
                last_transaction->getDateTime().toUtf8().size();
    }
}

Types::Message::MessageType StockInfo::type() const
{
    return Types::Message::MessageType::RESPONSE_STOCK_INFO;
}

void StockInfo::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);
    out << _stockId;
    send(_bestBuyOrder.get(), out);
    send(_bestSellOrder.get(), out);
    send(_lastTransaction.get(), out);
}

Types::Message::MessageLengthType StockInfo::length() const
{

    return  sizeof(Types::Message::MessageType) + sizeof(_stockId) +
            getSerializedLength(_bestBuyOrder.get()) + getSerializedLength(_bestSellOrder.get()) +
            getSerializedLength(_lastTransaction.get());
}


}
}
