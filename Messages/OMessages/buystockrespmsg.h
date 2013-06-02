#ifndef BUYSTOCKRESPMSG_H
#define BUYSTOCKRESPMSG_H

#include "omessage.h"
#include "transaction.h"
#include <QIODevice>


/*
 *  Nie działa
 *
 *      --jam231
 */

class BuyStockRespMsg : public OMessage
{
    Transaction m_transaction;

    qint16 length() const;
public:
    BuyStockRespMsg(const Transaction& transaction);

    MessageType type() const;
    void send(QIODevice* connection);
};

#endif // BUYSTOCKRESPMSG_H
