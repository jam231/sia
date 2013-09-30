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

    qint16 length() const;
    MessageType type() const;
};
}
}
#endif // GETMYSTOCKSMSG_H
