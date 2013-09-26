#include "subscribestockmsg.h"

namespace NetworkProtocol {

SubscribeStockMsg::SubscribeStockMsg(QDataStream& in)
{
    if(in.device()->bytesAvailable() < (sizeof(m_stockId)))
        throw InvalidDataInMsg();

    in >> m_stockId;
}

IOMessage::MessageType SubscribeStockMsg::type() const
{
    return SUBSCRIBE_STOCK;
}

qint32 SubscribeStockMsg::getStockId() const
{
    return m_stockId;
}

}
