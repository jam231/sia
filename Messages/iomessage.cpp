#include "iomessage.h"

IOMessage::IOMessage()
{}

IOMessage::MessageType IOMessage::toType(int n)
{
    switch(n)
    {
        case REGISTER_USER_REQ : return REGISTER_USER_REQ;
        case REGISTER_USER_RESP : return REGISTER_USER_RESP;
        case LOGIN_USER : return LOGIN_USER;
        case SUBSCRIBE_STOCK : return SUBSCRIBE_STOCK;
        case UNSUBSCRIBE_STOCK : return UNSUBSCRIBE_STOCK;
        case SELL_STOCK_REQ : return SELL_STOCK_REQ;
        case SELL_STOCK_RESP : return SELL_STOCK_RESP;
        case BUY_STOCK_REQ : return BUY_STOCK_REQ;
        case BUY_STOCK_RESP : return BUY_STOCK_RESP;
        case GET_STOCKS : return GET_STOCKS;
        case LIST_OF_STOCKS : return LIST_OF_STOCKS;
        case CHANGE_PRICE : return CHANGE_PRICE;
        case UNRECOGNIZED_USER : return UNRECOGNIZED_USER;
        default : return UNDEFINED;
    };
}
