#ifndef UNSUBSCRIBESTOCKMSG_H
#define UNSUBSCRIBESTOCKMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
class NETWORKPROTOCOLSHARED_EXPORT UnsubscribeStock : public Request
{
    DTO::Types::StockIdType m_stockId;

public:
    UnsubscribeStock(QDataStream& msg);

    DTO::Types::Message::MessageLengthType length() const;

    DTO::Types::Message::MessageType type() const;
    DTO::Types::StockIdType getStockId() const;

};
}
}
#endif // UNSUBSCRIBESTOCKMSG_H
