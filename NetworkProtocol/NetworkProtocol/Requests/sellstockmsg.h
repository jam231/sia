#ifndef SELLSTOCKMSG_H
#define SELLSTOCKMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
class NETWORKPROTOCOLSHARED_EXPORT SellStock : public Request
{
    DTO::Types::StockIdType m_stockId;
    DTO::Types::AmountType m_amount;
    DTO::Types::PriceType m_price;


public:
    SellStock(QDataStream& msg);

    DTO::Types::StockIdType getStockId() const;
    DTO::Types::AmountType getAmount() const;
    DTO::Types::PriceType getPrice() const;

    DTO::Types::Message::MessageType type() const;
    DTO::Types::MessageLengthType length() const;

    static const QString getMessageName();
};
}
}
#endif // SELLSTOCKMSG_H
