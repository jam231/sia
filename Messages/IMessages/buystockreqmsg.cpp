#include "buystockreqmsg.h"


BuyStockReqMsg::BuyStockReqMsg(QDataStream& in) //: IMessage()
{

    if(in.device()->bytesAvailable() < (sizeof(m_stockId) +
                                        sizeof(m_amount) +
                                        sizeof(m_price)))
        throw InvalidDataInMsg();

    in >> m_stockId;
    in >> m_amount;
    in >> m_price;
}

IOMessage::MessageType BuyStockReqMsg::type() const
{
    return SELL_STOCK_REQ;
}

qint32 BuyStockReqMsg::getAmount() const
{
    return m_amount;
}

qint32 BuyStockReqMsg::getPrice() const
{
    return m_price;
}

qint32 BuyStockReqMsg::getStockId() const
{
    return m_stockId;
}

qint16 BuyStockReqMsg::length() const
{
    return -1;
}
/*
Offer BuyStockReqMsg::offer() const
{
    return m_offer;
}
*/
