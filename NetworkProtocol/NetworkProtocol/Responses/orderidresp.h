#ifndef ORDERIDRESP_H
#define ORDERIDRESP_H

#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT OrderIdResp : public Response
{
    qint32 m_orderId;

public:
    OrderIdResp(qint32 orderId);

    qint16 length() const;
    void send(QIODevice *connection);
    Message::MessageType type() const;
};

}
}
#endif // ORDERIDRESP_H
