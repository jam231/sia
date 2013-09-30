#include "getmystocksrespmsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

GetMyStocksRespMsg::GetMyStocksRespMsg()
{
}

Message::MessageType GetMyStocksRespMsg::type() const
{
    return GET_MY_STOCKS_RESP;
}

void GetMyStocksRespMsg::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);
    out << static_cast<Types::MessageLengthType>(m_stocks.size());
    for(int i = 0; i < m_stocks.size(); ++i)
    {
        out << m_stocks[i].first
            << m_stocks[i].second;
    }
}

Types::MessageLengthType GetMyStocksRespMsg::length() const
{
    return sizeof(MessageType) + sizeof(Types::MessageLengthType) +
            m_stocks.size() * (sizeof(Types::StockIdType) + sizeof(Types::AmountType));
}

void GetMyStocksRespMsg::addStock(qint32 stockId, qint32 amount)
{
    m_stocks.push_back(qMakePair(stockId, amount));
}

}
}
