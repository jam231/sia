#include "getmyordermsg.h"


namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

GetMyOrderMsg::GetMyOrderMsg(QDataStream& in) : Request(in)
{
}

Types::Message::MessageType GetMyOrderMsg::type() const
{
    return Types::Message::MessageType::GET_MY_ORDERS;
}

Types::MessageLengthType GetMyOrderMsg::length() const
{
    return sizeof(Types::Message::MessageType);
}

const QString GetMyOrderMsg::getMessageName()
{
    return Request::getMessageName() + " :: GetMyOrderMsg";
}

}
}
