#include "shownewordermsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

ShowNewOrder::ShowNewOrder(Types::OrderIdType orderId,
                           Types::Order::OrderType orderType,
                           Types::StockIdType stockId,
                           Types::AmountType amount, Types::PriceType price)
    : ShowNewOrder(std::move(GlobalUtilities::getLogger()), orderId, orderType,
                   stockId, amount, price)
{}

ShowNewOrder::ShowNewOrder(std::shared_ptr<AbstractLogger> logger,
                           Types::OrderIdType orderId,
                           Types::Order::OrderType orderType,
                           Types::StockIdType stockId,
                           Types::AmountType amount, Types::PriceType price)
    : _order(std::move(logger), orderId, orderType, stockId, amount, price)
{
    // No need to validate the arguments - ShowNewOrder constructor delegates it to
    // constructor of Order.
}

ShowNewOrder::ShowNewOrder(Order order) : _order(order)
{
}

Types::Message::MessageType ShowNewOrder::type() const
{
    return Types::Message::MessageType::RESPONSE_SHOW_NEW_ORDER;
}

void ShowNewOrder::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);
    out << _order;
}

Types::Message::MessageLengthType ShowNewOrder::length() const
{
    return Response::length() + _order.lengthSerialized();
}

const Order& ShowNewOrder::getOrder() const
{
    return _order;
}

}
}
