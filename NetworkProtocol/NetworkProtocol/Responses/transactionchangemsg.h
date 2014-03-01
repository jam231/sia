#ifndef TRANSACTIONCHANGEMSG_H
#define TRANSACTIONCHANGEMSG_H

#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{

/*
 *  Responses :: TransactionChange
 *
 *  This message is for notifying user about semi-realization of one of his orders.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1><stock_id : 4><amount : 4><price : 4><date_time_str_length : 2><date_time_str : date_time_str_length>
 *
 *  where
 *      message_type              = MessageType::RESPONSE_TRANSACTION_CHANGE
 */
class NETWORKPROTOCOLSHARED_EXPORT TransactionChange : public Response
{
    DTO::Types::StockIdType _stockId;
    DTO::Types::AmountType _amount;
    DTO::Types::PriceType _price;
    QString _dateTime;

public:
    TransactionChange(DTO::Types::StockIdType, DTO::Types::AmountType,
                      DTO::Types::PriceType, QString);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

    void send(QIODevice *connection);
};

}
}
#endif // TRANSACTIONCHANGEMSG_H
