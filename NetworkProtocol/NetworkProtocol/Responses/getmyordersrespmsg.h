#ifndef GETMYORDERSRESPMSG_H
#define GETMYORDERSRESPMSG_H


#include "response.h"

#include "DataTransferObjects/order.h"

#include <QVector>
#include <QPair>

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT  GetMyOrdersRespMsg : public Response
{    
    QVector<DTO::Order*> m_orders;

public:
    GetMyOrdersRespMsg();

    DTO::Types::MessageType type() const;

    DTO::Types::MessageLengthType length() const;

    void send(QIODevice* connection);

    void addOrder(DTO::Types::OrderIdType orderId, DTO::Types::OrderType m_transactionType,
                  DTO::Types::StockIdType m_stockId, DTO::Types::AmountType m_amount,
                  DTO::Types::PriceType m_price);

    void addOrder(DTO::Order* order);

    static const QString getMessageName();

    virtual ~GetMyOrdersRespMsg();
};

}
}
#endif // GETMYORDERSRESPMSG_H
