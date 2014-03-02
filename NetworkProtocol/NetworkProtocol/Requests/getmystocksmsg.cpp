#include "getmystocksmsg.h"

#include "networkprotocol_utilities.h"


namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

GetMyStocks::GetMyStocks()
{
}

GetMyStocks::GetMyStocks(QDataStream&)
{
}

Types::Message::MessageType GetMyStocks::type() const
{
    return Types::Message::MessageType::REQUEST_GET_MY_STOCKS;
}

DTO::Types::Message::MessageLengthType GetMyStocks::length() const
{
    return 0;
}


}
}
