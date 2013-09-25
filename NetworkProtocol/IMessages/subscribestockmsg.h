#ifndef SUBSCRIBESTOCKMSG_H
#define SUBSCRIBESTOCKMSG_H

#include "imessage.h"

namespace NetworkProtocol {

class NETWORKPROTOCOLSHARED_EXPORT SubscribeStockMsg : public IMessage
{
    qint32 m_stockId;

    qint16 length() const;
public:
    SubscribeStockMsg(QDataStream& msg);

    MessageType type() const;
    qint32 getStockId() const;
};

}
#endif // SUBSCRIBESTOCKMSG_H
