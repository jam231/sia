#include "bestordermsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

BestOrderMsg::BestOrderMsg(Types::OrderType type, Types::StockIdType stockId,
                           Types::AmountType amount, Types::PriceType price)
    : m_bestOrder(BestOrder(type, stockId, amount, price))
{
}

BestOrderMsg::BestOrderMsg(BestOrder bestOrder)
    : m_bestOrder(bestOrder)
{
}

Message::MessageType BestOrderMsg::type() const
{
    return BEST_ORDER;
}

Types::MessageLengthType BestOrderMsg::length() const
{
    return sizeof(MessageType) + BestOrder::lengthInBytes;
}

void BestOrderMsg::send(QIODevice *connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);

    out << m_bestOrder;
}

}
}
