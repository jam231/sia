#ifndef SUBSCRIBESTOCKMSG_H
#define SUBSCRIBESTOCKMSG_H

#include "imessage.h"

#include <QIODevice>

/*
 *  Nie działa
 *
 *
 */

class SubscribeStockMsg : public IMessage
{
    qint32 m_stockId;
public:
    SubscribeStockMsg(QIODevice* msg);

    MessageType type() const;
    qint32 stockId() const;
};

#endif // SUBSCRIBESTOCKMSG_H
