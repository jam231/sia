#ifndef CANCELORDERMSG_H
#define CANCELORDERMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
class NETWORKPROTOCOLSHARED_EXPORT CancelOrderMsg : public Request
{
    qint32 m_orderId;

public:
    CancelOrderMsg(QDataStream& in);

    qint16 length() const;
    MessageType type() const;
    qint32 getOrderId() const;
};
}
}
#endif // CANCELORDERMSG_H
