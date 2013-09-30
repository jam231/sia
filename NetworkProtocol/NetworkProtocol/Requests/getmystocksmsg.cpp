#include "getmystocksmsg.h"


namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

GetMyStocksMsg::GetMyStocksMsg(QDataStream& in) : Request(in)
{
}

Types::MessageType GetMyStocksMsg::type() const
{
    return Types::MessageType::GET_MY_STOCKS;
}

DTO::Types::MessageLengthType GetMyStocksMsg::length() const
{
    return sizeof(Types::MessageType);
}

}
}
