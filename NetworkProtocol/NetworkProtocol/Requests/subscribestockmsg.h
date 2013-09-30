#ifndef SUBSCRIBESTOCKMSG_H
#define SUBSCRIBESTOCKMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
class NETWORKPROTOCOLSHARED_EXPORT SubscribeStockMsg : public Request
{
    DTO::Types::StockIdType m_stockId;

public:
    SubscribeStockMsg(QDataStream& msg);

    DTO::Types::MessageLengthType length() const;

    DTO::Types::MessageType type() const;
    DTO::Types::StockIdType getStockId() const;
};
}
}
#endif // SUBSCRIBESTOCKMSG_H
