#ifndef GETSTOCKINFOMSG_H
#define GETSTOCKINFOMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
class NETWORKPROTOCOLSHARED_EXPORT GetStockInfoMsg : public Request
{
    qint32 m_stockId;
public:
    GetStockInfoMsg(QDataStream& in);

    qint16 length() const;
    MessageType type() const;
    qint32 getStockId() const;
};
}
}
#endif // GETSTOCKINFOMSG_H
