#include "listofordersmsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

ListOfOrders::ListOfOrders()
{
}

Types::Message::MessageLengthType ListOfOrders::length() const
{
    Types::Message::MessageLengthType order_length_in_bytes = 0;
    if(_orders.size() > 0)
    {
        order_length_in_bytes = _orders.first()->lengthSerialized();
    }

    return Response::length() + _orders.size() * order_length_in_bytes;
}

Types::Message::MessageType ListOfOrders::type() const
{
    return Types::Message::MessageType::RESPONSE_LIST_OF_ORDERS;
}

void ListOfOrders::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);
    out << static_cast<Types::Message::MessageLengthType>(_orders.size());
    for(int i = 0; i < _orders.size(); ++i)
    {
        out << _orders[i]->getOrderId()
            << _orders[i]->getOrderType()
            << _orders[i]->getStockId()
            << _orders[i]->getAmount()
            << _orders[i]->getPrice();
    }
}

void ListOfOrders::addOrder(Types::OrderIdType orderId,
                            Types::Order::OrderType _orderType,
                            Types::StockIdType stockId,
                            Types::AmountType amount,
                            Types::PriceType price)
{
    _orders.push_back(std::move(std::shared_ptr<Order>(
                          new Order(orderId, _orderType, stockId,
                                    amount, price))));
}

void ListOfOrders::addOrder(std::shared_ptr<Order> order)
{
    _orders.push_back(std::move(order));
}


}
}
