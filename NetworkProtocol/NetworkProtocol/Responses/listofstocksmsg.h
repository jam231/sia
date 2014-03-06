#ifndef SHOWUSERSTOCKSMSG_H
#define SHOWUSERSTOCKSMSG_H

#include "response.h"

#include <QVector>
#include <QPair>

namespace NetworkProtocol
{
namespace Responses
{
/*
 *  Responses :: ListOfStocks
 *
 *  This message is for sending user information about stocks he owns.
 *
 *
 *  Message format:
 *  <message_length : 2><message_type : 1><stock_count : 2>(<stock_id : 4><amount : 4>)^stock_count
 *
 *  where
 *       message_type       = MessageType::RESPONSE_LIST_OF_STOCKS
 */
class NETWORKPROTOCOLSHARED_EXPORT ListOfStocks : public Response
{
    QVector<QPair<DTO::Types::StockIdType, DTO::Types::AmountType> > _stocks;

public:

    ListOfStocks();

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

    void send(QIODevice* connection);

    void addStock(DTO::Types::StockIdType stockId, DTO::Types::AmountType amount);
};

}
}
#endif // SHOWUSERSTOCKSMSG_H
