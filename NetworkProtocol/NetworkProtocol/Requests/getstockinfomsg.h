#ifndef GETSTOCKINFOMSG_H
#define GETSTOCKINFOMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
class NETWORKPROTOCOLSHARED_EXPORT GetStockInfoMsg : public Request
{
    DTO::Types::StockIdType m_stockId;
public:
    GetStockInfoMsg(QDataStream& in);

    DTO::Types::MessageLengthType length() const;
    DTO::Types::MessageType type() const;
    DTO::Types::StockIdType getStockId() const;
};
}
}
#endif // GETSTOCKINFOMSG_H
