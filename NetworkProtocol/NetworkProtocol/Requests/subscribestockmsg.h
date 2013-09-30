#ifndef SUBSCRIBESTOCKMSG_H
#define SUBSCRIBESTOCKMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
class NETWORKPROTOCOLSHARED_EXPORT SubscribeStockMsg : public Request
{
    qint32 m_stockId;

public:
    SubscribeStockMsg(QDataStream& msg);

    qint16 length() const;

    MessageType type() const;
    qint32 getStockId() const;
};
}
}
#endif // SUBSCRIBESTOCKMSG_H
