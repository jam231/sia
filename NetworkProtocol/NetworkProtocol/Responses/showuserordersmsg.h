#ifndef SHOWUSERORDERSMSG_H
#define SHOWUSERORDERSMSG_H


#include "response.h"

#include "DataTransferObjects/order.h"

#include <QVector>
#include <QPair>

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT  ShowUserOrders : public Response
{    
    QVector<DTO::Order*> m_orders;

public:
    ShowUserOrders();

    DTO::Types::Message::MessageType type() const;

    DTO::Types::MessageLengthType length() const;

    void send(QIODevice* connection);

    void addOrder(DTO::Types::OrderIdType orderId, DTO::Types::Order::OrderType m_orderType,
                  DTO::Types::StockIdType m_stockId, DTO::Types::AmountType m_amount,
                  DTO::Types::PriceType m_price);

    void addOrder(DTO::Order* order);

    static const QString getMessageName();

    virtual ~ShowUserOrders();
};

}
}
#endif // SHOWUSERORDERSMSG_H
