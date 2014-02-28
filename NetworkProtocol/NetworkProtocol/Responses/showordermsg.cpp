#include "showordermsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

ShowOrder::ShowOrder(Types::OrderIdType orderId, Types::Order::OrderType orderType,
                     Types::StockIdType stockId, Types::AmountType amount, Types::PriceType price)
    : _order(orderId, orderType, stockId, amount, price)
{
}

ShowOrder::ShowOrder(Order order) : _order(order)
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
    out << _order;
}

Types::Message::MessageLengthType ShowOrder::length() const
{
    return sizeof(Types::Message::MessageType) + _order.lengthSerialized();
}

const Order& ShowOrder::getOrder() const
{
    return _order;
}



}
}
