#include "buystockrespmsg.h"

#include <QDataStream>

BuyStockRespMsg::BuyStockRespMsg(const Transaction& transaction)
    : OMessage(), m_transaction(transaction)
{
}

qint32 BuyStockRespMsg::length() const
{
    //8 bajtów na typ i dlugosc
    return 8 + sizeof(BuyStockRespMsg);
}

IOMessage::MessageType BuyStockRespMsg::type() const
{
    return BUY_STOCK_RESP;
}

void BuyStockRespMsg::send(QIODevice* connection)
{
    QDataStream tmpStream(connection);
    tmpStream<<length();
    tmpStream<<static_cast<qint32>(type());
    tmpStream<<m_transaction;
}
