#include "getstocksmsg.h"

GetStocksMsg::GetStocksMsg() : IMessage()
{}

IOMessage::MessageType GetStocksMsg::type() const
{
    return GET_STOCKS;
}
