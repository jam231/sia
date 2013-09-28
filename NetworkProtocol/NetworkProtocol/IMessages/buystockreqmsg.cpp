#include "buystockreqmsg.h"


namespace NetworkProtocol {


BuyStockReqMsg::BuyStockReqMsg(QDataStream& in)
{

    if(in.device()->bytesAvailable() < (sizeof(m_stockId) +
                                        sizeof(m_amount) +
                                        sizeof(m_price)))
        throw InvalidDataInMsg();

    in >> m_stockId;
    in >> m_amount;
    in >> m_price;
    // Mam wielka nadzieje, ze w bazie te rzeczy sa sprawdane,
    // a juz na pewno m_stockId
    if(m_amount <= 0 || m_price <= 0)
        throw InvalidDataInMsg();
}

IOMessage::MessageType BuyStockReqMsg::type() const
{
    return BUY_STOCK_REQ;
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

}
