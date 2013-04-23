#include "sellstockrespmsg.h"

#include <QDataStream>

SellStockRespMsg::SellStockRespMsg(const Transaction& transaction)
    : OMessage(), m_transaction(transaction)
{
}

qint32 SellStockRespMsg::length() const
{
    //8 bajtów na typ i dlugosc
    return 8 + sizeof(SellStockRespMsg);
}

IOMessage::MessageType SellStockRespMsg::type() const
{
    return SELL_STOCK_RESP;
}

void SellStockRespMsg::send(QIODevice* connection)
{
    QDataStream tmpStream(connection);
    tmpStream<<length();
    tmpStream<<static_cast<qint32>(type());
    tmpStream<<m_transaction;
}

