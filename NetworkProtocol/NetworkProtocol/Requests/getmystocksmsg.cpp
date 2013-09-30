#include "getmystocksmsg.h"


namespace NetworkProtocol
{
namespace Requests
{

GetMyStocksMsg::GetMyStocksMsg(QDataStream& in) : Request(in)
{
}

Message::MessageType GetMyStocksMsg::type() const
{
    return GET_MY_STOCKS;
}

qint16 GetMyStocksMsg::length() const
{
    return sizeof(MessageType);
}

}
}
