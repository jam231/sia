#include "unsubscribestockmsg.h"


UnsubscribeStockMsg::UnsubscribeStockMsg(QDataStream& in) //: IMessage()
{
    if(in.device()->bytesAvailable() < (sizeof(m_stockId)))
        throw InvalidDataInMsg();

    in >> m_stockId;
}

IOMessage::MessageType UnsubscribeStockMsg::type() const
{
    return UNSUBSCRIBE_STOCK;
}

qint32 UnsubscribeStockMsg::getStockId() const
{
    return m_stockId;
}
