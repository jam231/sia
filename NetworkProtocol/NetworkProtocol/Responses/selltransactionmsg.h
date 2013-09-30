#ifndef SELLTRANSACTIONMSG_H
#define SELLTRANSACTIONMSG_H

#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT SellTransactionMsg : public Response
{
    qint32 m_amount;
    qint32 m_orderId;

public:
    SellTransactionMsg(qint32 m_orderId, qint32 m_amount);

    qint16 length() const;
    void send(QIODevice *connection);
    Message::MessageType type() const;
};

}
}
#endif // SELLTRANSACTIONMSG_H
