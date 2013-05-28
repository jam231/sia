#ifndef UNSUBSCRIBESTOCKMSG_H
#define UNSUBSCRIBESTOCKMSG_H

#include "imessage.h"

#include <QDataStream>

/*
 *  Nie działa
 *
 *      --jam231
 */

class UnsubscribeStockMsg : public IMessage
{
    qint32 m_stockId;
public:
    UnsubscribeStockMsg(QDataStream& msg);

    MessageType type() const;
    qint32 getStockId() const;
};

#endif // UNSUBSCRIBESTOCKMSG_H
