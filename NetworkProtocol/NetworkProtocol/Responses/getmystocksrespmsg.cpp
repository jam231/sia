#include "getmystocksrespmsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

GetMyStocksRespMsg::GetMyStocksRespMsg()
{
}

Types::MessageType GetMyStocksRespMsg::type() const
{
    return Types::MessageType::GET_MY_STOCKS_RESP;
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
    return sizeof(Types::MessageType) +
                sizeof(Types::MessageLengthType) +
                m_stocks.size() * (sizeof(Types::StockIdType) + sizeof(Types::AmountType));
}

void GetMyStocksRespMsg::addStock(Types::StockIdType stockId, Types::AmountType amount)
{
    m_stocks.push_back(qMakePair(stockId, amount));
}

}
}
