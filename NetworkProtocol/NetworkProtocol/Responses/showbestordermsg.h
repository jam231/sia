#ifndef BESTORDERMSG_H
#define BESTORDERMSG_H

#include "response.h"
#include "DataTransferObjects/bestorder.h"

namespace NetworkProtocol
{
namespace Responses
{
/*
 *  Responses :: ShowBestOrder
 *
 *  This message is for sending user information about new best order
 *  for stock with stockId.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1><best_order : 17>
 *
 *  where
 *      message_type         = MessageType::RESPONSE_SHOW_BEST_ORDER
 *      <best_order : 17>    = <order_type : 1><stock_id : 4><amount : 4><price : 4>
 */
class NETWORKPROTOCOLSHARED_EXPORT ShowBestOrder : public Response
{
    DTO::BestOrder _bestOrder;
public:
    ShowBestOrder(DTO::Types::Order::OrderType, DTO::Types::StockIdType,
                  DTO::Types::AmountType, DTO::Types::PriceType);
    ShowBestOrder(DTO::BestOrder);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;


    void send(QIODevice *connection);
};

}
}
#endif // BESTORDERMSG_H
