#include "stockinfomsg.h"

#include <memory>



#include <stdexcept>
#include <utilities.h>
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
    if(stockId.value >= 0)
    {
        LOG_TRACE(QString("stockId(%1) <= 0 == false").arg(_stockId.value));
        throw std::invalid_argument("stockId <= 0.");
    }
    if(_bestBuyOrder == nullptr ||
       (_bestBuyOrder->getOrderType() == Order::BUY && _bestBuyOrder->getStockId() == _stockId))
    {
        LOG_TRACE(QString("Best buy order is invalid - wrong stock id (should be %1 is %2) or order type(%3).")
                  .arg(_stockId.value).arg(_bestBuyOrder->getStockId().value)
                  .arg(_bestBuyOrder->getOrderType()));
        throw std::invalid_argument("Best buy order is invalid - wrong stock Id or order type.");
    }
    if(_bestSellOrder == nullptr ||
       (_bestSellOrder->getOrderType() == Order::SELL && _bestSellOrder->getStockId() == _stockId))
    {
        LOG_TRACE(QString("Best sell order is invalid - wrong stock id (should be %1 is %2) or order type(%3).")
                  .arg(_stockId.value).arg(_bestSellOrder->getStockId().value)
                  .arg(_bestSellOrder->getOrderType()));
        throw std::invalid_argument("Best sell order is invalid - wrong stock Id or order type.");
    }
}

void StockInfo::send(BestOrder* bestOrder, QDataStream &out)
{
    if(bestOrder == nullptr)
    {
        out << 0;
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
        out << 0;
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
        return  sizeof(PriceType) +
                sizeof(Types::Message::MessageLengthType) +
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
