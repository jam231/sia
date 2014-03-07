#include "getmyordersmsg.h"

#include "networkprotocol_utilities.h"


namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;
GetMyOrders::GetMyOrders()
{
}

GetMyOrders::GetMyOrders(QDataStream&)
{
}

Types::Message::MessageType GetMyOrders::type() const
{
    return Types::Message::MessageType::REQUEST_GET_MY_ORDERS;
}

Types::Message::MessageLengthType GetMyOrders::length() const
{
    return Request::length();
}


}
}
