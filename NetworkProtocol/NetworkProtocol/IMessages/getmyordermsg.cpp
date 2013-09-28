#include "getmyordermsg.h"


namespace NetworkProtocol {

GetMyOrderMsg::GetMyOrderMsg()
{
}

IOMessage::MessageType GetMyOrderMsg::type() const
{
    return GET_MY_ORDERS;
}

}
