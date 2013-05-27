#ifndef NEWTRANSACTION_H
#define NEWTRANSACTION_H



#include "omessage.h"

#include <QIODevice>

enum TransactionType : qint8
{
    BUY = 1,
    SELL = 1
};

class NewTransaction : public OMessage
{
    TransactionType m_transactionType;
    qint32 m_stockId;
    qint32 m_amount;
    qint32 m_price;
    qint32 length() const;
public:

    NewTransaction(TransactionType, qint32, qint32, qint32);
    void send(QIODevice *connection);
    IOMessage::MessageType type() const;
};

#endif // NEWTRANSACTION_H
