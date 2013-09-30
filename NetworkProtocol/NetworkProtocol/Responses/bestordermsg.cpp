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

Types::MessageType BestOrderMsg::type() const
{
    return Types::MessageType::BEST_ORDER;
}

const QString BestOrderMsg::getMessageName()
{
    return Response::getMessageName() + ":: BestOrderMsg";
}

Types::MessageLengthType BestOrderMsg::length() const
{
    return sizeof(Types::MessageType) + m_bestOrder.lengthInBytes();
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