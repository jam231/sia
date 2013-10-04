#include "showordermsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

ShowOrder::ShowOrder(Types::OrderIdType orderId, Types::Order::OrderType orderType,
                     Types::StockIdType stockId, Types::AmountType amount, Types::PriceType price)
    : m_order(orderId, orderType, stockId, amount, price)
{
}

ShowOrder::ShowOrder(Order order) : m_order(order)
{
}

Types::Message::MessageType ShowOrder::type() const
{
    return Types::Message::MessageType::RESPONSE_SHOW_ORDER;
}

void ShowOrder::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);
    out << m_order;
}

Types::MessageLengthType ShowOrder::length() const
{
    return sizeof(Types::Message::MessageType) + m_order.lengthInBytes();
}

const Order& ShowOrder::getOrder() const
{
    return m_order;
}

const QString ShowOrder::getMessageName()
{
    return Response::getMessageName() + " :: ShowOrder";
}

}
}
