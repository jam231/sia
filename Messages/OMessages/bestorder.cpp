#include "bestorder.h"

BestOrder::BestOrder(OrderType type, qint32 stockId,
                     qint32 amount, qint32 price) :
    OrderMsg(type, stockId, amount, price)
{}

IOMessage::MessageType BestOrder::type() const
{
    return BEST_ORDER;
}
