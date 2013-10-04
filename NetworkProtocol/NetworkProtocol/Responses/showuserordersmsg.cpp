#include "showuserordersmsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

ShowUserOrders::ShowUserOrders()
{
}

Types::MessageLengthType ShowUserOrders::length() const
{
    return sizeof(Types::Message::MessageType) +
           sizeof(Types::MessageLengthType) +
            std::accumulate(m_orders.begin(), m_orders.end(),
                            static_cast<Types::MessageLengthType>(0),
                            [](Types::MessageLengthType len, Order* order) -> Types::MessageLengthType
                                   {
                                        return len + order->lengthInBytes();
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
    out << static_cast<Types::MessageLengthType>(m_orders.size());
    for(int i = 0; i < m_orders.size(); ++i)
        out << m_orders[i]->getOrderId()
            << m_orders[i]->getOrderType()
            << m_orders[i]->getStockId()
            << m_orders[i]->getAmount()
            << m_orders[i]->getPrice();

}

void ShowUserOrders::addOrder(Types::OrderIdType orderId, Types::Order::OrderType m_orderType,
                                  Types::StockIdType stockId, Types::AmountType amount,
                                  Types::PriceType price)
{
    m_orders.push_back(new Order(orderId, m_orderType, stockId, amount, price));
}

void ShowUserOrders::addOrder(Order* order)
{
    m_orders.push_back(order);
}

ShowUserOrders::~ShowUserOrders()
{
    foreach(Order* order, m_orders)
    {
        delete order;
    }
}

const QString ShowUserOrders::getMessageName()
{
    return Response::getMessageName() + " :: ShowUserOrders";
}

}
}
