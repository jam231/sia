#include "listofstocksmsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

ListOfStocks::ListOfStocks()
{
}


Types::Message::MessageType ListOfStocks::type() const
{
    return Types::Message::MessageType::RESPONSE_LIST_OF_STOCKS;
}

void ListOfStocks::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);
    out << static_cast<Types::Message::MessageLengthType>(_stocks.size());
    for(int i = 0; i < _stocks.size(); ++i)
    {
        out << _stocks[i].first << _stocks[i].second;
    }
}

Types::Message::MessageLengthType ListOfStocks::length() const
{
    return sizeof(Types::Message::MessageType) +
           sizeof(Types::Message::MessageLengthType) +
           _stocks.size() * (sizeof(Types::StockIdType) + sizeof(Types::AmountType));
}

void ListOfStocks::addStock(Types::StockIdType stockId, Types::AmountType amount)
{
    _stocks.push_back(qMakePair(stockId, amount));
}



}
}
