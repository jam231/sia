#ifndef GETMYORDERMSG_H
#define GETMYORDERMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
class NETWORKPROTOCOLSHARED_EXPORT GetMyOrderMsg : public Request
{
public:
    GetMyOrderMsg(QDataStream& in);

    qint16 length() const;
    MessageType type() const;
};
}
}
#endif // GETMYORDERMSG_H
