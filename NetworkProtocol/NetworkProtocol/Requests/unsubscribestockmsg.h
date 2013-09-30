#ifndef UNSUBSCRIBESTOCKMSG_H
#define UNSUBSCRIBESTOCKMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
class NETWORKPROTOCOLSHARED_EXPORT UnsubscribeStockMsg : public Request
{
    DTO::Types::StockIdType m_stockId;

public:
    UnsubscribeStockMsg(QDataStream& msg);

    DTO::Types::MessageLengthType length() const;

    DTO::Types::MessageType type() const;
    DTO::Types::StockIdType getStockId() const;
};
}
}
#endif // UNSUBSCRIBESTOCKMSG_H
