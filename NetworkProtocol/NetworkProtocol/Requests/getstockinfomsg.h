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
    DTO::Types::Message::MessageType type() const;
    DTO::Types::StockIdType getStockId() const;

    static const QString getMessageName();
};
}
}
#endif // GETSTOCKINFOMSG_H
