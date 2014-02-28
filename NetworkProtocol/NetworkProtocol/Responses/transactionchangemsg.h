#ifndef TRANSACTIONCHANGEMSG_H
#define TRANSACTIONCHANGEMSG_H

#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{

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
