#ifndef TRANSACTIONCHANGE_H
#define TRANSACTIONCHANGE_H

#include "omessage.h"

namespace NetworkProtocol {

class NETWORKPROTOCOLSHARED_EXPORT TransactionChange : public OMessage
{
    qint32 m_stockId;
    qint32 m_amount;
    qint32 m_price;
    QString m_date;

    qint16 length() const;
public:
    TransactionChange(qint32, qint32, qint32, QString);
    void send(QIODevice *connection);
    IOMessage::MessageType type() const;
};

}
#endif // TRANSACTIONCHANGE_H
