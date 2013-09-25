#ifndef UNSUBSCRIBESTOCKMSG_H
#define UNSUBSCRIBESTOCKMSG_H

#include "imessage.h"

namespace NetworkProtocol {

class  UnsubscribeStockMsg : public IMessage
{
    qint32 m_stockId;

    qint16 length() const;
public:
    UnsubscribeStockMsg(QDataStream& msg);

    MessageType type() const;
    qint32 getStockId() const;
};

}
#endif // UNSUBSCRIBESTOCKMSG_H
