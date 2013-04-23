#include "sellstockreqmsg.h"

#include <QDataStream>

SellStockReqMsg::SellStockReqMsg(QIODevice* msg) : IMessage()
{
    QDataStream tmpStream;
    tmpStream>>m_offer;
}

IOMessage::MessageType SellStockReqMsg::type() const
{
    return SELL_STOCK_REQ;
}

Offer SellStockReqMsg::offer() const
{
    return m_offer;
}
