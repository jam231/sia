#ifndef GETMYSTOCKSMSG_H
#define GETMYSTOCKSMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
/// TODO:   Add description with serialization format.
class NETWORKPROTOCOLSHARED_EXPORT GetMyStocks : public Request
{
public:
    GetMyStocks(QDataStream& in);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

};
}
}
#endif // GETMYSTOCKSMSG_H
