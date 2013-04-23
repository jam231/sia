#include "buystockreqmsg.h"

#include <QDataStream>

BuyStockReqMsg::BuyStockReqMsg(QIODevice* msg) : IMessage()
{
    QDataStream tmpStream(msg);
    tmpStream>>m_offer;
}

IOMessage::MessageType BuyStockReqMsg::type() const
{
    return SELL_STOCK_REQ;
}

Offer BuyStockReqMsg::offer() const
{
    return m_offer;
}
