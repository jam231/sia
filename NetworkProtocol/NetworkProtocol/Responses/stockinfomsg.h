#ifndef STOCKINFOMSG_H
#define STOCKINFOMSG_H

#include "response.h"
#include "DataTransferObjects/lasttransaction.h"

#include <QPair>


namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT StockInfo : public Response
{
    QPair<DTO::Types::AmountType, DTO::Types::PriceType> m_bestBuyOrder;
    QPair<DTO::Types::AmountType, DTO::Types::PriceType> m_bestSellOrder;
    DTO::LastTransaction m_lastTransaction;

    DTO::Types::StockIdType m_stockId;
public:
    StockInfo(DTO::Types::StockIdType stockId,
                        QPair<DTO::Types::AmountType, DTO::Types::PriceType> bestBuyOrder
                            = qMakePair(DTO::Types::AmountType(0), DTO::Types::PriceType(0)),
                        QPair<DTO::Types::AmountType, DTO::Types::PriceType> bestSellOrder
                            = qMakePair(DTO::Types::AmountType(0), DTO::Types::PriceType(0)),
                        DTO::LastTransaction lastTransaction  = DTO::LastTransaction());

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

    void send(QIODevice* connection);
};

}
}
#endif // STOCKINFOMSG_H
