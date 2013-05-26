#include "sellstockreqmsg.h"


SellStockReqMsg::SellStockReqMsg(QDataStream& in) //: IMessage()
{
    if(in.device()->bytesAvailable() < (sizeof(m_stockId) +
                                        sizeof(m_amount) +
                                        sizeof(m_price)))
        throw InvalidDataInMsg();

    in >> m_stockId;
    in >> m_amount;
    in >> m_price;
}

IOMessage::MessageType SellStockReqMsg::type() const
{
    return SELL_STOCK_REQ;
}


qint32 SellStockReqMsg::getAmount() const
{
    return m_amount;
}

qint32 SellStockReqMsg::getPrice() const
{
    return m_price;
}

qint32 SellStockReqMsg::getStockId() const
{
    return m_stockId;
}

/*
Offer SellStockReqMsg::offer() const
{
    return m_offer;
}
*/
