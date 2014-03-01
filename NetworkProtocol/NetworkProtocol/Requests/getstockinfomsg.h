#ifndef GETSTOCKINFOMSG_H
#define GETSTOCKINFOMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
/// TODO:   Add description with serialization format.
class NETWORKPROTOCOLSHARED_EXPORT GetStockInfo : public Request
{
    DTO::Types::StockIdType _stockId;
public:
    GetStockInfo(QDataStream& in);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;
    DTO::Types::StockIdType getStockId() const;

};
}
}
#endif // GETSTOCKINFOMSG_H
