#include "getmyordermsg.h"


namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

GetMyOrderMsg::GetMyOrderMsg(QDataStream& in) : Request(in)
{
}

Types::MessageType GetMyOrderMsg::type() const
{
    return Types::MessageType::GET_MY_ORDERS;
}

Types::MessageLengthType GetMyOrderMsg::length() const
{
    return sizeof(Types::MessageType);
}

}
}
