#include "getmyordermsg.h"


namespace NetworkProtocol
{
namespace Requests
{

GetMyOrderMsg::GetMyOrderMsg(QDataStream& in) : Request(in)
{
}

Message::MessageType GetMyOrderMsg::type() const
{
    return GET_MY_ORDERS;
}

qint16 GetMyOrderMsg::length() const
{
    return sizeof(MessageType);
}

}
}
