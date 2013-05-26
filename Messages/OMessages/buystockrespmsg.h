#ifndef BUYSTOCKRESPMSG_H
#define BUYSTOCKRESPMSG_H

#include "omessage.h"
#include "transaction.h"
#include <QIODevice>

class BuyStockRespMsg : public OMessage
{
    Transaction m_transaction;

    qint32 length() const;
public:
    BuyStockRespMsg(const Transaction& transaction);

    MessageType type() const;
    void send(QIODevice* connection);
};

#endif // BUYSTOCKRESPMSG_H
