#include "getmystocksmsg.h"


namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

GetMyStocksMsg::GetMyStocksMsg(QDataStream& in) : Request(in)
{
}

Types::Message::MessageType GetMyStocksMsg::type() const
{
    return Types::Message::MessageType::GET_MY_STOCKS;
}

DTO::Types::MessageLengthType GetMyStocksMsg::length() const
{
    return sizeof(Types::Message::MessageType);
}

const QString GetMyStocksMsg::getMessageName()
{
    return Request::getMessageName() + " :: GetMyStocksMsg";
}

}
}
