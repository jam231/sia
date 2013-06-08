#ifndef ORDERIDRESP_H
#define ORDERIDRESP_H

#include "omessage.h"

class OrderIdResp : public OMessage
{
    qint32 m_orderId;

    qint16 length() const;
public:
    OrderIdResp(qint32 orderId);

    void send(QIODevice *connection);
    IOMessage::MessageType type() const;
};

#endif // ORDERIDRESP_H
