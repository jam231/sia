#ifndef BESTORDER_H
#define BESTORDER_H

#include "ordermsg.h"

class BestOrder : public OrderMsg
{
public:
    BestOrder(OrderType, qint32, qint32, qint32);
    // TODO:
    // Brzydze sie siebie, nalezy to usunac ! --jam231
    BestOrder() : OrderMsg() {}
    IOMessage::MessageType type() const;
};

#endif // BESTORDER_H
