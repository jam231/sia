#ifndef BESTORDER_H
#define BESTORDER_H

#include "ordermsg.h"

class BestOrderMsg : public OrderMsg
{
public:
    BestOrderMsg(Order::OrderType, qint32, qint32, qint32);
    BestOrderMsg(Order order);
    // TODO:
    // Brzydze sie siebie, nalezy to usunac ! --jam231
    BestOrderMsg() : OrderMsg() {}
    IOMessage::MessageType type() const;
};

#endif // BESTORDER_H
