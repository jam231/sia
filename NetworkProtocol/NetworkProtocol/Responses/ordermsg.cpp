#include "ordermsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

OrderMsg::OrderMsg(Types::OrderIdType orderId, Types::Order::OrderType orderType,
                   Types::StockIdType stockId, Types::AmountType amount, Types::PriceType price)
    : m_order(orderId, orderType, stockId, amount, price)
{
}

OrderMsg::OrderMsg(DTO::Order order) : m_order(order)
{
}

Types::Message::MessageType OrderMsg::type() const
{
    return Types::Message::MessageType::ORDER;
}

void OrderMsg::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);
    out << m_order;
}

Types::MessageLengthType OrderMsg::length() const
{
    return sizeof(Types::Message::MessageType) + m_order.lengthInBytes();
}

const Order& OrderMsg::getOrder() const
{
    return m_order;
}

const QString OrderMsg::getMessageName()
{
    return Response::getMessageName() + " :: OrderMsg";
}

}
}
