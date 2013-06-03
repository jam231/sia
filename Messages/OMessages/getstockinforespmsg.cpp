#include "getstockinforespmsg.h"

GetStockInfoRespMsg::GetStockInfoRespMsg(qint32 stockId, QPair<qint32, qint32> bestBuyOrder, QPair<qint32, qint32> bestSellOrder, QPair<qint32, qint32> lastTransaction)
    : m_bestBuyOrder(bestBuyOrder), m_bestSellOrder(bestSellOrder), m_lastTransaction(lastTransaction)
{
    m_stockId = stockId;
}

IOMessage::MessageType GetStockInfoRespMsg::type() const
{
    return GET_STOCK_INFO_RESP;
}
void GetStockInfoRespMsg::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);
    out << m_stockId <<  m_bestBuyOrder << m_bestSellOrder << m_lastTransaction;
}

qint16 GetStockInfoRespMsg::length() const
{
    return sizeof(MessageType) + sizeof(qint32) + 3 * (2*sizeof(qint32));
}

