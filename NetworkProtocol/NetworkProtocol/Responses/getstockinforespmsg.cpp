#include "getstockinforespmsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

GetStockInfoRespMsg::GetStockInfoRespMsg(Types::StockIdType stockId,
                                         QPair<Types::AmountType, Types::PriceType> bestBuyOrder,
                                         QPair<Types::AmountType, Types::PriceType> bestSellOrder,
                                         LastTransaction lastTransaction)
    : m_bestBuyOrder(bestBuyOrder),
      m_bestSellOrder(bestSellOrder),
      m_lastTransaction(lastTransaction)
{
    m_stockId = stockId;
}

Message::MessageType GetStockInfoRespMsg::type() const
{
    return GET_STOCK_INFO_RESP;
}
void GetStockInfoRespMsg::send(QIODevice* connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    QByteArray dateTime = m_lastTransaction.getDateTime().toUtf8();

    sendHeader(out);
    out << m_stockId
        << m_bestBuyOrder
        << m_bestSellOrder
        << m_lastTransaction.getPrice()
        << static_cast<Types::MessageLengthType>(dateTime.size());
    connection->write(dateTime);
}

DTO::Types::MessageLengthType GetStockInfoRespMsg::length() const
{
    return sizeof(MessageType) + sizeof(m_stockId) +
            2 * (2 * sizeof(qint32)) +
            sizeof(Types::MessageLengthType) + m_lastTransaction.lengthInBytes();
}

}
}
