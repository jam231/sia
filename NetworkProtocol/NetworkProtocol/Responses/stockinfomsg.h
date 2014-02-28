#ifndef STOCKINFOMSG_H
#define STOCKINFOMSG_H

#include "response.h"
#include "DataTransferObjects/lasttransaction.h"
#include "DataTransferObjects/bestorder.h"

#include <memory>


namespace NetworkProtocol
{
namespace Responses
{
/*
 *  This message is for sending a basic information about the stock with stock_id.
 *
 *  Message format:
 *  <message_length : 16><message_type : 8><stock_id : qint32> <best_buy_offer : ?><best_sell_offer : ?><last_transaction : ?>
 *
 *  where
 *      <best_buy_offer : ?>  when no best buy offer    = <-1 : 32>
 *      <best_buy_offer : ?>  otherwise                 = <amount : 32><price : 32>
 *      <best_sell_offer : ?> when no best sell offer   = <-1 : 32>
 *      <best_sell_offer : ?> otherwise                 = <amount : 32><price : 32>
 *      <best_buy_offer : ?>  when no last transaction  = <-1 : 32>
 *      <best_buy_offer : ?>  otherwise                 = <price : 32><data_time_str_length : 16><date_time_str : data_time_str_length>
 *
 *  0 value is a safe choice for guard value, i.e. amount or price cannot be equal to 0.
 *
 *
 */
class NETWORKPROTOCOLSHARED_EXPORT StockInfo : public Response
{
protected:
    std::shared_ptr<DTO::BestOrder> _bestBuyOrder;
    std::shared_ptr<DTO::BestOrder> _bestSellOrder;
    std::shared_ptr<DTO::LastTransaction> _lastTransaction;
    DTO::Types::StockIdType _stockId;

    void send(DTO::BestOrder*, QDataStream&);
    void send(DTO::LastTransaction*, QDataStream&);

    DTO::Types::Message::MessageLengthType getSerializedLength(DTO::BestOrder*) const;
    DTO::Types::Message::MessageLengthType getSerializedLength(DTO::LastTransaction*) const;
public:
    StockInfo(DTO::Types::StockIdType stockId,
              std::shared_ptr<DTO::BestOrder> bestBuyOrder = nullptr,
              std::shared_ptr<DTO::BestOrder> bestSellOrder = nullptr,
              std::shared_ptr<DTO::LastTransaction> lastTransaction = nullptr);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

    void send(QIODevice* connection);
};

}
}
#endif // STOCKINFOMSG_H
