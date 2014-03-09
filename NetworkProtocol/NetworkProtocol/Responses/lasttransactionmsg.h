#ifndef LASTTRANSACTIONMSG_H
#define LASTTRANSACTIONMSG_H

#include "response.h"
#include "DataTransferObjects/lasttransaction.h"


#include "utilities.h"

#include <memory>

namespace NetworkProtocol
{
namespace Responses
{

/*
 *  Responses :: LastTransaction
 *
 *  This message if for sending information about last transaction.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1><stock_id : 4><amount : 4><price : 4><date_time_str_length : 2><date_time_str : date_time_str_length>
 *
 *  where
 *      message_type              = MessageType::RESPONSE_LAST_TRANSACTION (0x18)
 */
class NETWORKPROTOCOLSHARED_EXPORT LastTransaction : public Response
{

    DTO::Types::StockIdType _stockId;
    DTO::LastTransaction    _lastTransaction;

public:
    LastTransaction(std::shared_ptr<AbstractLogger> logger,
                    DTO::Types::StockIdType, DTO::Types::AmountType,
                    DTO::Types::PriceType, QString);
    LastTransaction(std::shared_ptr<AbstractLogger> logger,
                    DTO::Types::StockIdType, DTO::LastTransaction);
    LastTransaction(DTO::Types::StockIdType, DTO::Types::AmountType,
                    DTO::Types::PriceType, QString);
    LastTransaction(DTO::Types::StockIdType, DTO::LastTransaction);

    DTO::LastTransaction getLastTransaction() const;
    DTO::Types::StockIdType getStockId() const;

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

    void send(QIODevice *connection);
};

}
}
#endif // LASTTRANSACTIONMSG_H
