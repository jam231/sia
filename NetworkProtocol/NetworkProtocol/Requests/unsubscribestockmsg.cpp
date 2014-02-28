#include "unsubscribestockmsg.h"

namespace NetworkProtocol
{
namespace Requests
{
using namespace DTO;

UnsubscribeStock::UnsubscribeStock(QDataStream& in) : Request(in)
{
    in >> _stockId;
}


Types::Message::MessageType UnsubscribeStock::type() const
{
    return Types::Message::MessageType::REQUEST_UNSUBSCRIBE_STOCK;
}

Types::StockIdType UnsubscribeStock::getStockId() const
{
    return _stockId;
}

Types::Message::MessageLengthType UnsubscribeStock::length() const
{
    return sizeof(Types::Message::MessageType) +
           sizeof(_stockId);
}

}
}
