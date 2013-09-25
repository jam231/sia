#include "getmystocksmsg.h"


namespace NetworkProtocol {

GetMyStocksMsg::GetMyStocksMsg() : IMessage()
{
}

IOMessage::MessageType GetMyStocksMsg::type() const
{
    return GET_MY_STOCKS;
}

}
