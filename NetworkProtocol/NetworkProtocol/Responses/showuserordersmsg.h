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
    QVector<DTO::Order*> _orders;

public:
    ShowUserOrders();

    DTO::Types::Message::MessageType type() const;

    DTO::Types::Message::MessageLengthType length() const;

    void send(QIODevice* connection);

    void addOrder(DTO::Types::OrderIdType orderId, DTO::Types::Order::OrderType _orderType,
                  DTO::Types::StockIdType _stockId, DTO::Types::AmountType _amount,
                  DTO::Types::PriceType _price);

    void addOrder(DTO::Order* order);

    virtual ~ShowUserOrders();
};

}
}
#endif // SHOWUSERORDERSMSG_H
