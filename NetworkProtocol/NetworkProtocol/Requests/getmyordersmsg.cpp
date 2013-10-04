#include "getmyordersmsg.h"


namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

GetMyOrders::GetMyOrders(QDataStream& in) : Request(in)
{
}

Types::Message::MessageType GetMyOrders::type() const
{
    return Types::Message::MessageType::REQUEST_GET_MY_ORDERS;
}

Types::MessageLengthType GetMyOrders::length() const
{
    return sizeof(Types::Message::MessageType);
}

const QString GetMyOrders::getMessageName()
{
    return Request::getMessageName() + " :: GetMyOrders";
}

}
}
