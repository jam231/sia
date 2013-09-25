#ifndef NEWTRANSACTION_H
#define NEWTRANSACTION_H

#include "omessage.h"

#include "DataTransferObjects/order.h"

namespace NetworkProtocol {

class  OrderMsg : public OMessage
{
    Order m_order;

    qint16 length() const;
public:
    qint32 getStockId();
    OrderMsg(Order::OrderType, qint32, qint32, qint32);
    OrderMsg(Order order);
    // TODO:
    // Brzydze sie siebie, nalezy to usunac ! --jam231
    OrderMsg();
    virtual ~OrderMsg() {}
    void send(QIODevice *connection);
    IOMessage::MessageType type() const;

};

}
#endif // NEWTRANSACTION_H
