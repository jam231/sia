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

Types::MessageType GetStockInfoRespMsg::type() const
{
    return Types::MessageType::GET_STOCK_INFO_RESP;
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

Types::MessageLengthType GetStockInfoRespMsg::length() const
{
    return  sizeof(Types::MessageType) + sizeof(m_stockId) +
            sizeof(Types::AmountType) + sizeof(Types::PriceType) + // bestBuyOrder
            sizeof(Types::AmountType) + sizeof(Types::PriceType) + // bestSellOrder
            sizeof(Types::MessageLengthType) + m_lastTransaction.lengthInBytes();
}

const QString GetStockInfoRespMsg::getMessageName()
{
    return Response::getMessageName() + " :: GetStockInfoRespMsg";
}
}
}
