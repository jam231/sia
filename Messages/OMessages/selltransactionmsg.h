#ifndef SELLTRANSACTIONMSG_H
#define SELLTRANSACTIONMSG_H

#include "omessage.h"

#include <QIODevice>

class SellTransactionMsg : public OMessage
{
    qint32 m_amount;
    qint32 m_orderId;

    qint32 length() const;
public:
    SellTransactionMsg(qint32 m_orderId, qint32 m_amount);
    void send(QIODevice *connection);
    IOMessage::MessageType type() const;
};

#endif // SELLTRANSACTIONMSG_H
