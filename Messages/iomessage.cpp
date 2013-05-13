
#include "iomessage.h"


IOMessage::IOMessage()
{}

IOMessage::MessageType IOMessage::toType(qint8 msgId)
{
    switch(msgId)
    {
        case REGISTER_USER_REQ : return REGISTER_USER_REQ;
        case REGISTER_USER_RESP_OK : return REGISTER_USER_RESP_OK;
        case REGISTER_USER_RESP_FAIL : return REGISTER_USER_RESP_FAIL;
        case LOGIN_USER_REQ : return LOGIN_USER_REQ;
        case LOGIN_USER_RESP_OK : return LOGIN_USER_RESP_OK;
        case LOGIN_USER_RESP_FAIL : return LOGIN_USER_RESP_FAIL;
        case SUBSCRIBE_STOCK : return SUBSCRIBE_STOCK;
        case UNSUBSCRIBE_STOCK : return UNSUBSCRIBE_STOCK;
        case SELL_STOCK_REQ : return SELL_STOCK_REQ;
        case SELL_STOCK_RESP : return SELL_STOCK_RESP;
        case BUY_STOCK_REQ : return BUY_STOCK_REQ;
        case BUY_STOCK_RESP : return BUY_STOCK_RESP;
        case GET_STOCKS : return GET_STOCKS;
        case LIST_OF_STOCKS : return LIST_OF_STOCKS;
        case CHANGE_PRICE : return CHANGE_PRICE;
        default : return UNDEFINED;
    };
}
