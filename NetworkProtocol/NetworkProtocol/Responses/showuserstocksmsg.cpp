#include "showuserstocksmsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

ShowUserStocks::ShowUserStocks()
{
}

Types::Message::MessageType ShowUserStocks::type() const
{
    return Types::Message::MessageType::RESPONSE_SHOW_USER_STOCKS;
}

void ShowUserStocks::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);
    out << static_cast<Types::Message::MessageLengthType>(m_stocks.size());
    for(int i = 0; i < m_stocks.size(); ++i)
    {
        out << m_stocks[i].first
            << m_stocks[i].second;
    }
}

Types::Message::MessageLengthType ShowUserStocks::length() const
{
    return sizeof(Types::Message::MessageType) +
                sizeof(Types::Message::MessageLengthType) +
                m_stocks.size() * (sizeof(Types::StockIdType) + sizeof(Types::AmountType));
}

void ShowUserStocks::addStock(Types::StockIdType stockId, Types::AmountType amount)
{
    m_stocks.push_back(qMakePair(stockId, amount));
}



}
}
