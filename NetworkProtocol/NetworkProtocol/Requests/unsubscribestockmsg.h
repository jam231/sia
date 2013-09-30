#ifndef UNSUBSCRIBESTOCKMSG_H
#define UNSUBSCRIBESTOCKMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
class NETWORKPROTOCOLSHARED_EXPORT UnsubscribeStockMsg : public Request
{
    qint32 m_stockId;

public:
    UnsubscribeStockMsg(QDataStream& msg);

    qint16 length() const;

    MessageType type() const;
    qint32 getStockId() const;
};
}
}
#endif // UNSUBSCRIBESTOCKMSG_H
