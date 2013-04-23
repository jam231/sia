#ifndef SELLSTOCKRESPMSG_H
#define SELLSTOCKRESPMSG_H

#include "omessage.h"
#include "transaction.h"

#include <QIODevice>

class SellStockRespMsg : public OMessage
{
    Transaction m_transaction;

    qint32 length() const;
public:
    SellStockRespMsg(const Transaction& transaction);

    MessageType type() const;
    void send(QIODevice* connection);
};

#endif // SELLSTOCKRESPMSG_H
