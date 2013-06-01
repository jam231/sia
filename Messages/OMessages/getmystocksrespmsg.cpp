#include "getmystocksrespmsg.h"

#include <QDataStream>

GetMyStocksRespMsg::GetMyStocksRespMsg()
{
}

IOMessage::MessageType GetMyStocksRespMsg::type() const
{
    return GET_MY_STOCKS_RESP;
}
void GetMyStocksRespMsg::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    out << length() << static_cast<qint8>(type())
        << static_cast<qint32>(m_stocks.size());
    for(int i = 0; i < m_stocks.size(); ++i)
    {
        out << static_cast<qint32>(m_stocks[i].first)
            << static_cast<qint32>(m_stocks[i].second);
    }
}

qint32 GetMyStocksRespMsg::length() const
{
    return sizeof(type()) + sizeof(qint32) + m_stocks.size()*2*sizeof(qint32);
}

void GetMyStocksRespMsg::addStock(qint32 stockId, qint32 amount)
{
    m_stocks.push_back(qMakePair(stockId, amount));
}
