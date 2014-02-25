#include "getstockinfomsg.h"


namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

GetStockInfo::GetStockInfo(QDataStream &in) : Request(in)
{
    in >> m_stockId;
}

Types::Message::MessageType GetStockInfo::type() const
{
    return Types::Message::MessageType::REQUEST_GET_STOCK_INFO;
}

DTO::Types::StockIdType GetStockInfo::getStockId() const
{
    return m_stockId;
}

Types::Message::MessageLengthType GetStockInfo::length() const
{
    return sizeof(Types::Message::MessageType) +
           sizeof(m_stockId);
}


}
}
