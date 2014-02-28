#include "stockinfomsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

StockInfo::StockInfo (Types::StockIdType stockId,
                                         QPair<Types::AmountType, Types::PriceType> bestBuyOrder,
                                         QPair<Types::AmountType, Types::PriceType> bestSellOrder,
                                         LastTransaction lastTransaction)
    : m_bestBuyOrder(bestBuyOrder),
      m_bestSellOrder(bestSellOrder),
      m_lastTransaction(lastTransaction)
{
    m_stockId = stockId;
}

Types::Message::MessageType StockInfo::type() const
{
    return Types::Message::MessageType::RESPONSE_STOCK_INFO;
}

void StockInfo::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    QByteArray dateTime = m_lastTransaction.getDateTime().toUtf8();

    sendHeader(out);
    out << m_stockId
        << m_bestBuyOrder
        << m_bestSellOrder
        << m_lastTransaction.getPrice()
        << static_cast<Types::Message::MessageLengthType>(dateTime.size());
    connection->write(dateTime);
}

Types::Message::MessageLengthType StockInfo::length() const
{
    return  sizeof(Types::Message::MessageType) + sizeof(m_stockId) +
            sizeof(Types::AmountType) + sizeof(Types::PriceType) + // bestBuyOrder
            sizeof(Types::AmountType) + sizeof(Types::PriceType) + // bestSellOrder
            sizeof(Types::Message::MessageLengthType) + m_lastTransaction.lengthSerialized();
}


}
}
