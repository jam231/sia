#include "subscribestockmsg.h"

#include <QDataStream>

SubscribeStockMsg::SubscribeStockMsg(QIODevice* msg) : IMessage()
{
    QDataStream tmpStream;
    tmpStream>>m_stockId;
}

IOMessage::MessageType SubscribeStockMsg::type() const
{
    return SUBSCRIBE_STOCK;
}

qint32 SubscribeStockMsg::stockId() const
{
    return m_stockId;
}
