#include "bestorder.h"

namespace NetworkProtocol {

BestOrderMsg::BestOrderMsg(DTO::Order::OrderType type, qint32 stockId, qint32 amount, qint32 price)
    : OrderMsg(type, stockId, amount, price)
{
}

BestOrderMsg::BestOrderMsg(DTO::Order order)
    : OrderMsg(order)
{
}

IOMessage::MessageType BestOrderMsg::type() const
{
    return BEST_ORDER;
}

}
