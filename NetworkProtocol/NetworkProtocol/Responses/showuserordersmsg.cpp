#include "showuserordersmsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

ShowUserOrders::ShowUserOrders()
{
}

Types::Message::MessageLengthType ShowUserOrders::length() const
{
    return sizeof(Types::Message::MessageType) +
           sizeof(Types::Message::MessageLengthType) +
            std::accumulate(_orders.begin(), _orders.end(),
                            static_cast<Types::Message::MessageLengthType>(0),
                            [](Types::Message::MessageLengthType len, Order* order) -> Types::Message::MessageLengthType
                                   {
                                        return len + order->lengthSerialized();
                                    }
                            );
}

Types::Message::MessageType ShowUserOrders::type() const
{
    return Types::Message::MessageType::RESPONSE_SHOW_USER_ORDERS;
}

void ShowUserOrders::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);
    out << static_cast<Types::Message::MessageLengthType>(_orders.size());
    for(int i = 0; i < _orders.size(); ++i)
        out << _orders[i]->getOrderId()
            << _orders[i]->getOrderType()
            << _orders[i]->getStockId()
            << _orders[i]->getAmount()
            << _orders[i]->getPrice();

}

void ShowUserOrders::addOrder(Types::OrderIdType orderId, Types::Order::OrderType _orderType,
                                  Types::StockIdType stockId, Types::AmountType amount,
                                  Types::PriceType price)
{
    _orders.push_back(new Order(orderId, _orderType, stockId, amount, price));
}

void ShowUserOrders::addOrder(Order* order)
{
    _orders.push_back(order);
}

ShowUserOrders::~ShowUserOrders()
{
    foreach(Order* order, _orders)
    {
        delete order;
    }
}



}
}
