#include "getmystocksmsg.h"


namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

GetMyStocks::GetMyStocks(QDataStream& in) : Request(in)
{
}

Types::Message::MessageType GetMyStocks::type() const
{
    return Types::Message::MessageType::REQUEST_GET_MY_STOCKS;
}

DTO::Types::MessageLengthType GetMyStocks::length() const
{
    return sizeof(Types::Message::MessageType);
}

const QString GetMyStocks::getMessageName()
{
    return Request::getMessageName() + " :: GetMyStocks";
}

}
}
