#include "unsubscribestockmsg.h"

#include <QDataStream>

UnsubscribeStockMsg::UnsubscribeStockMsg(QIODevice* msg) : IMessage()
{
    QDataStream tmpStream;
    tmpStream>>m_stockId;
}

IOMessage::MessageType UnsubscribeStockMsg::type() const
{
    return UNSUBSCRIBE_STOCK;
}

qint32 UnsubscribeStockMsg::stockId() const
{
    return m_stockId;
}
