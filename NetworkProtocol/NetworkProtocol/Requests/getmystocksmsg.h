#ifndef GETMYSTOCKSMSG_H
#define GETMYSTOCKSMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
class NETWORKPROTOCOLSHARED_EXPORT GetMyStocksMsg : public Request
{
public:
    GetMyStocksMsg(QDataStream& in);

    DTO::Types::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

    static const QString getMessageName();
};
}
}
#endif // GETMYSTOCKSMSG_H
