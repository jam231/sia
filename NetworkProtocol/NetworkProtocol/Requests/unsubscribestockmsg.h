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

    DTO::Types::Message::MessageType type() const;
    DTO::Types::StockIdType getStockId() const;

    static const QString getMessageName();
};
}
}
#endif // UNSUBSCRIBESTOCKMSG_H
