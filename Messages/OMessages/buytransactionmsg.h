#ifndef BUYTRANSACTIONMSG_H
#define BUYTRANSACTIONMSG_H

#include "omessage.h"

#include <QIODevice>

class BuyTransactionMsg : public OMessage
{
    qint32 m_amount;
    qint32 m_orderId;

    qint32 length() const;
public:

    BuyTransactionMsg(qint32 orderId, qint32 amount);
    void send(QIODevice *connection);
    IOMessage::MessageType type() const;
};


#endif // BUYTRANSACTIONMSG_H