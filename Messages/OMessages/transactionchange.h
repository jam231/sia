#ifndef TRANSACTIONCHANGE_H
#define TRANSACTIONCHANGE_H

#include "omessage.h"

#include <QIODevice>

class TransactionChange : public OMessage
{
    qint32 m_stockId;
    qint32 m_amount;
    qint32 m_price;
    QString m_date;

    qint32 length() const;
public:
    TransactionChange(qint32, qint32, qint32, QString);
    void send(QIODevice *connection);
    IOMessage::MessageType type() const;
};


#endif // TRANSACTIONCHANGE_H
