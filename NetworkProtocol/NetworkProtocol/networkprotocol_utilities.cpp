#include "networkprotocol_utilities.h"

namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;
using namespace DTO::Types;
using namespace DTO::Types::Message;

std::shared_ptr<Request> fromStream(QDataStream& stream)
{

    Message::MessageLengthType length = readLength(stream);
    Message::MessageType       type   = readType(stream);

    // Read length bytes even if request is malformed.
    QDataStream serialized_request(stream.device()->read(length));
    Request* request;
    try{
    switch(type)
    {
    case REQUEST_REGISTER_USER:
        request = new RegisterUser(serialized_request);
        break;
    case REQUEST_LOGIN_USER:
        request = new LoginUser(serialized_request);
        break;
    case REQUEST_SELL_STOCK_ORDER:
        request = new SellStock(serialized_request);
        break;
    case REQUEST_BUY_STOCK_ORDER:
        request = new BuyStock(serialized_request);
        break;
    case REQUEST_SUBSCRIBE_STOCK:
        request = new SubscribeStock(serialized_request);
        break;
    case REQUEST_UNSUBSCRIBE_STOCK:
        request = new UnsubscribeStock(serialized_request);
        break;
    case REQUEST_GET_MY_STOCKS:
        request = new GetMyStocks(serialized_request);
        break;
    case REQUEST_GET_MY_ORDERS:
        request = new GetMyOrders(serialized_request);
        break;
    case REQUEST_GET_STOCK_INFO:
        request = new GetStockInfo(serialized_request);
        break;
    case REQUEST_CANCEL_ORDER:
        request = new CancelOrder(serialized_request);
        break;
    case REQUEST_COMPANY_STATUS:
        request = new CompanyStatus(serialized_request);
        break;
    //case REQUEST_SESSION_STATUS:
    //    request = new SessionStatus(serialized_request);
    //    break;
    default:
        GLOBAL_LOG_ERROR(QString("Invalid type(%1). This shouldn't happpen.").arg(type));
        throw InvalidRequestType();
        break;
    };
    }catch(std::invalid_argument& e)
    {
        GLOBAL_LOG_TRACE(e.what());
        throw InvalidRequestBody(e.what());
    }

    return std::shared_ptr<Request>(request);
}

Message::MessageLengthType getLength(QIODevice* data)
{
    if(data->bytesAvailable() < 2)
    {
        GLOBAL_LOG_TRACE(QString("Not enough bytes in device. Should be %1 is %2")
                  .arg(sizeof(Message::MessageLengthType)).arg(data->bytesAvailable()));
        return -1;
    }
    Message::MessageLengthType message_length;

    QDataStream in(data->peek(sizeof(Message::MessageLengthType)));

    in >> message_length;

    return message_length;
}

Message::MessageLengthType readLength(QDataStream& stream)
{
    Message::MessageLengthType request_length = getLength(stream.device());
    if(request_length < stream.device()->bytesAvailable())
    {
        GLOBAL_LOG_TRACE(QString("Request is incomplete. Request supposed length: %1"\
                          " Available bytes in stream %2.")
                  .arg(request_length).arg(stream.device()->bytesAvailable()));
        throw IncompleteRequest(request_length);
    }
    stream.skipRawData(sizeof(Message::MessageLengthType));
    return request_length;
}

Message::MessageType readType_NoEx(QDataStream& stream)
{
    if(stream.device()->bytesAvailable() < 1)
    {
        GLOBAL_LOG_TRACE(QString("Can't read %1 byte(s) from empty stream.")
                  .arg(sizeof(Message::MessageType)));

        return Message::MessageType::MESSAGE_UNDEFINED;
    }
    Message::MessageType type;
    stream >> type;

    return type;
}

Message::MessageType readType(QDataStream& stream)
{
    Message::MessageType type = readType_NoEx(stream);
    if(type == Message::MESSAGE_UNDEFINED)
    {
        GLOBAL_LOG_TRACE(QString("Invalid request type."));
        throw InvalidRequestType();
    }

    return type;
}

}
}
