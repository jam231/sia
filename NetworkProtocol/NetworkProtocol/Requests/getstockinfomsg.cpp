#include "getstockinfomsg.h"


namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

GetStockInfoMsg::GetStockInfoMsg(QDataStream &in) : Request(in)
{
    in >> m_stockId;
}

Types::Message::MessageType GetStockInfoMsg::type() const
{
    return Types::Message::MessageType::GET_STOCK_INFO;
}

DTO::Types::StockIdType GetStockInfoMsg::getStockId() const
{
    return m_stockId;
}

Types::MessageLengthType GetStockInfoMsg::length() const
{
    return sizeof(Types::Message::MessageType) +
           sizeof(m_stockId);
}

const QString GetStockInfoMsg::getMessageName()
{
    return Request::getMessageName() + " :: GetStockInfoMsg";
}

}
}
