#ifndef BESTORDER_H
#define BESTORDER_H

#include "order.h"

class BestOrder : public Order
{
public:
    BestOrder(OrderType, qint32, qint32, qint32);
    // TODO:
    // Brzydze sie siebie, nalezy to usunac ! --jam231
    BestOrder() : Order() {}
    IOMessage::MessageType type() const;
};

#endif // BESTORDER_H
