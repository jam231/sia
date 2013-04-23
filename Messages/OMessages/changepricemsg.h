#ifndef CHANGEPRICEMSG_H
#define CHANGEPRICEMSG_H

#include "omessage.h"
#include "stock.h"

#include <QIODevice>

class ChangePriceMsg : public OMessage
{
    Stock m_stock;

    qint32 length() const;
public:
    ChangePriceMsg(const Stock& stock);

    MessageType type() const;
    void send(QIODevice* connection);
};

#endif // CHANGEPRICEMSG_H
