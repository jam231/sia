#ifndef BUYTRANSACTIONMSG_H
#define BUYTRANSACTIONMSG_H

#include "omessage.h"

namespace NetworkProtocol {

class NETWORKPROTOCOLSHARED_EXPORT BuyTransactionMsg : public OMessage
{
    qint32 m_amount;
    qint32 m_orderId;

    qint16 length() const;
public:

    BuyTransactionMsg(qint32 orderId, qint32 amount);
    void send(QIODevice *connection);
    IOMessage::MessageType type() const;
};

}

#endif // BUYTRANSACTIONMSG_H
