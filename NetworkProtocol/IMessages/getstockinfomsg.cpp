#include "getstockinfomsg.h"


namespace NetworkProtocol {

GetStockInfoMsg::GetStockInfoMsg(QDataStream &in)
{
    if(in.device()->bytesAvailable() < sizeof(m_stockId))
        throw InvalidDataInMsg();

    in >> m_stockId;
}

IOMessage::MessageType GetStockInfoMsg::type() const
{
    return GET_STOCK_INFO;
}

qint32 GetStockInfoMsg::getStockId() const
{
    return m_stockId;
}

}
