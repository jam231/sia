#ifndef GETSTOCKINFORESPMSG_H
#define GETSTOCKINFORESPMSG_H

#include "response.h"
#include "DataTransferObjects/lasttransaction.h"

#include <QPair>


namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT GetStockInfoRespMsg : public Response
{
    QPair<DTO::Types::AmountType, DTO::Types::PriceType> m_bestBuyOrder;
    QPair<DTO::Types::AmountType, DTO::Types::PriceType> m_bestSellOrder;
    DTO::LastTransaction m_lastTransaction;

    DTO::Types::StockIdType m_stockId;
public:
    GetStockInfoRespMsg(DTO::Types::StockIdType stockId,
                        QPair<qint32, qint32> bestBuyOrder    = qMakePair(0,0),
                        QPair<qint32, qint32> bestSellOrder   = qMakePair(0,0),
                        DTO::LastTransaction lastTransaction  = DTO::LastTransaction());

    DTO::Types::MessageLengthType length() const;
    DTO::Types::MessageType type() const;

    static const QString getMessageName();

    void send(QIODevice* connection);
};

}
}
#endif // GETSTOCKINFORESPMSG_H
