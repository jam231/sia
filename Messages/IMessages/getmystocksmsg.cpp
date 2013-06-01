#include "getmystocksmsg.h"

GetMyStocksMsg::GetMyStocksMsg() : IMessage()
{
}

IOMessage::MessageType GetMyStocksMsg::type() const
{
    return GET_MY_STOCKS;
}
