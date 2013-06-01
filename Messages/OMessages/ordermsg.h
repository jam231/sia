#ifndef NEWTRANSACTION_H
#define NEWTRANSACTION_H



#include "omessage.h"

#include <QIODevice>

enum OrderType : qint8
{
    UNDEFINED = 0,
    BUY = 1,
    SELL = 2
};

class OrderMsg : public OMessage
{
    OrderType m_transactionType;
    qint32 m_stockId;
    qint32 m_amount;
    qint32 m_price;
    qint32 length() const;
public:
    qint32 getStockId();
    OrderMsg(OrderType, qint32, qint32, qint32);
    // TODO:
    // Brzydze sie siebie, nalezy to usunac ! --jam231
    OrderMsg();
    virtual ~OrderMsg() {}
    void send(QIODevice *connection);
    IOMessage::MessageType type() const;

};

#endif // NEWTRANSACTION_H
