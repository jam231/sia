#ifndef BESTORDER_H
#define BESTORDER_H

#include "ordermsg.h"

namespace NetworkProtocol {

class NETWORKPROTOCOLSHARED_EXPORT  BestOrderMsg : public OrderMsg
{
public:
    BestOrderMsg(DTO::Order::OrderType, qint32, qint32, qint32);
    BestOrderMsg(DTO::Order order);
    // TODO:
    // Brzydze sie siebie, nalezy to usunac ! --jam231
    BestOrderMsg() : OrderMsg() {}
    IOMessage::MessageType type() const;
};

}
#endif // BESTORDER_H
