#ifndef CANCELORDERMSG_H
#define CANCELORDERMSG_H

#include <QDataStream>
#include "imessage.h"

class CancelOrderMsg : public IMessage
{
    qint32 m_orderId;

    qint16 length() const;
public:
    CancelOrderMsg(QDataStream& in);

    MessageType type() const;
    qint32 getOrderId() const;
};

#endif // CANCELORDERMSG_H
