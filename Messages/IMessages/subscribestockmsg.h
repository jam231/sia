#ifndef SUBSCRIBESTOCKMSG_H
#define SUBSCRIBESTOCKMSG_H

#include "imessage.h"

#include <QDataStream>

/*
 *  Nie działa
 *
 *      --jam231
 */

class SubscribeStockMsg : public IMessage
{
    qint32 m_stockId;
public:
    SubscribeStockMsg(QDataStream& msg);

    MessageType type() const;
    qint32 getStockId() const;
};

#endif // SUBSCRIBESTOCKMSG_H
