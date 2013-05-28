#ifndef NEWTRANSACTION_H
#define NEWTRANSACTION_H



#include "omessage.h"

#include <QIODevice>

enum OrderType : qint8
{
    BUY = 1,
    SELL = 1
};

class NewOrder : public OMessage
{
    OrderType m_transactionType;
    qint32 m_stockId;
    qint32 m_amount;
    qint32 m_price;
    qint32 length() const;
public:
    qint32 getStockId();
    NewOrder(OrderType, qint32, qint32, qint32);
    void send(QIODevice *connection);
    IOMessage::MessageType type() const;
};

#endif // NEWTRANSACTION_H
