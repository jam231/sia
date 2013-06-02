#include "getstockinforespmsg.h"

GetStockInfoRespMsg::GetStockInfoRespMsg(Order bestBuyOrder, Order bestSellOrder, Order lastOrder)
    : m_bestBuyOrder(bestBuyOrder), m_bestSellOrder(bestSellOrder), m_lastOrder(lastOrder)
{   }

IOMessage::MessageType GetStockInfoRespMsg::type() const
{
    return GET_STOCK_INFO_RESP;
}
void GetStockInfoRespMsg::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);
    out << m_bestBuyOrder << m_bestSellOrder << m_lastOrder;
}

qint16 GetStockInfoRespMsg::length() const
{
    return sizeof(MessageType) + 3 * (sizeof(qint8) + 3*sizeof(qint32));
}

