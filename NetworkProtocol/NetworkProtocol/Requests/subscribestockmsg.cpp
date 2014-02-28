#include "subscribestockmsg.h"

namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

SubscribeStock::SubscribeStock (QDataStream& in) : Request(in)
{
    in >> _stockId;
}

Types::Message::MessageType SubscribeStock::type() const
{
    return Types::Message::MessageType::REQUEST_SUBSCRIBE_STOCK;
}

Types::StockIdType SubscribeStock::getStockId() const
{
    return _stockId;
}

Types::Message::MessageLengthType SubscribeStock::length() const
{
    return sizeof(Types::Message::MessageType) +
           sizeof(_stockId);
}


}
}
