#ifndef UNSUBSCRIBESTOCKMSG_H
#define UNSUBSCRIBESTOCKMSG_H

#include "imessage.h"

#include <QIODevice>

class UnsubscribeStockMsg : public IMessage
{
    qint32 m_stockId;
public:
    UnsubscribeStockMsg(QIODevice* msg);

    MessageType type() const;
    qint32 stockId() const;
};

#endif // UNSUBSCRIBESTOCKMSG_H
