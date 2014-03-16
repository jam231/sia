#include "networkprotocol_utilities.h"

#include <assert.h>

namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;
using namespace DTO::Types;
using namespace DTO::Types::Message;

std::shared_ptr<Request> fromStream(QDataStream& stream)
{
    return fromStream(std::move(GlobalUtilities::getLogger()), stream);
}

std::shared_ptr<Request> fromStream(std::shared_ptr<AbstractLogger> logger,
                                    QDataStream& stream)
{

    Message::MessageLengthType length = tryReadLength(logger, stream);

    length -= 2;

    if(length < sizeof(Message::MessageType))
    {
        throw MalformedRequest("length < sizeof(Message::MessageType)");
    }
    Message::MessageType       type   = readType(logger, stream);

    // Read length bytes even if request is malformed.
    QDataStream serialized_request_body(stream.device()->read(length));
    Request* request;
    try{
    switch(type)
    {
    case REQUEST_REGISTER_USER:
        request = new RegisterUser(std::move(logger), serialized_request_body);
        break;
    case REQUEST_LOGIN_USER:
        request = new LoginUser(std::move(logger), serialized_request_body);
        break;
    case REQUEST_SELL_STOCK_ORDER:
        request = new SellStock(std::move(logger), serialized_request_body);
        break;
    case REQUEST_BUY_STOCK_ORDER:
        request = new BuyStock(std::move(logger), serialized_request_body);
        break;
    case REQUEST_SUBSCRIBE_STOCK:
        request = new SubscribeStock(std::move(logger), serialized_request_body);
        break;
    case REQUEST_UNSUBSCRIBE_STOCK:
        request = new UnsubscribeStock(std::move(logger), serialized_request_body);
        break;
    case REQUEST_GET_MY_STOCKS:
        request = new GetMyStocks(std::move(logger), serialized_request_body);
        break;
    case REQUEST_GET_MY_ORDERS:
        request = new GetMyOrders(std::move(logger), serialized_request_body);
        break;
    case REQUEST_GET_STOCK_INFO:
        request = new GetStockInfo(std::move(logger), serialized_request_body);
        break;
    case REQUEST_CANCEL_ORDER:
        request = new CancelOrder(std::move(logger), serialized_request_body);
        break;
    //case REQUEST_COMPANY_STATUS:
    //    request = new CompanyStatus(std::move(logger), serialized_request_body);
    //    break;
    //case REQUEST_SESSION_STATUS:
    //    request = new SessionStatus(std::move(logger), serialized_request_body);
    //    break;
    default:
        LOG_ERROR(logger, QString("Invalid type(%1). This shouldn't happpen.")
                         .arg(type));
        throw InvalidRequestType();
        break;
    };
    }catch(std::invalid_argument& e)
    {
        LOG_TRACE(logger, e.what());
        throw InvalidRequestBody(e.what());
    }

    return std::shared_ptr<Request>(request);
}

Message::MessageLengthType getLength(QIODevice* data)
{
    return getLength(GlobalUtilities::getLogger(), data);
}

Message::MessageLengthType getLength(std::shared_ptr<AbstractLogger> logger,
                                     QIODevice* data)
{
    if(data->bytesAvailable() < 2)
    {
        LOG_TRACE(logger,
                  QString("Not enough bytes in device. Should be %1 is %2")
                  .arg(sizeof(Message::MessageLengthType))
                  .arg(data->bytesAvailable()));
        return 0;
    }
    Message::MessageLengthType message_length;

    QDataStream in(data->peek(sizeof(Message::MessageLengthType)));

    in >> message_length;

    return message_length;
}


Message::MessageLengthType tryReadLength(std::shared_ptr<AbstractLogger> logger,
                                         QDataStream& stream)
{
    Message::MessageLengthType request_length = getLength(logger,
                                                          stream.device());
    if(request_length == 0)
    {
       LOG_TRACE(logger,
                  QString("Request is incomplete. Request should be at least %1 bytes"
                          " Available bytes in stream %2.")
                  .arg(sizeof(request_length))
                  .arg(stream.device()->bytesAvailable()));
        throw IncompleteRequest(sizeof(request_length));
    }
    if(request_length > stream.device()->bytesAvailable())
    {
        LOG_TRACE(logger,
                  QString("Request is incomplete. Supposed request length: %1."\
                          " Available bytes in stream %2.")
                  .arg(request_length)
                  .arg(stream.device()->bytesAvailable()));
        throw IncompleteRequest(request_length);
    }
    stream.skipRawData(sizeof(request_length));
    return request_length;
}


Message::MessageLengthType readLength(QDataStream& stream)
{
    return readLength(std::move(GlobalUtilities::getLogger()), stream);
}

Message::MessageLengthType readLength(std::shared_ptr<AbstractLogger> logger,
                                      QDataStream& stream)
{
    Message::MessageLengthType request_length = getLength(logger,
                                                          stream.device());
    if(request_length == 0)
    {
       LOG_TRACE(logger,
                  QString("Request is incomplete. Request should be at least %1 bytes"
                          " Available bytes in stream %2.")
                  .arg(sizeof(request_length))
                  .arg(stream.device()->bytesAvailable()));
        throw IncompleteRequest(sizeof(request_length));
    }  

    stream.skipRawData(sizeof(request_length));
    return request_length;
}

Message::MessageType readType_NoEx(QDataStream& stream)
{
    return readType_NoEx(std::move(GlobalUtilities::getLogger()), stream);
}

Message::MessageType readType_NoEx(std::shared_ptr<AbstractLogger> logger,
                                   QDataStream& stream)
{
    if(stream.device()->bytesAvailable() < 1)
    {
        LOG_TRACE(logger,
                  QString("Can't read %1 byte(s) from empty stream.")
                  .arg(sizeof(Message::MessageType)));

        return Message::MessageType::MESSAGE_UNDEFINED;
    }
    Message::MessageType type;
    stream >> type;

    return type;
}

Message::MessageType readType(QDataStream& stream)
{
    return readType(std::move(GlobalUtilities::getLogger()), stream);
}

Message::MessageType readType(std::shared_ptr<AbstractLogger> logger,
                              QDataStream& stream)
{
    Message::MessageType type = readType_NoEx(logger, stream);
    if(type == Message::MESSAGE_UNDEFINED)
    {
        LOG_TRACE(logger,
                  QString("Invalid request type."));
        throw InvalidRequestType();
    }

    return type;
}

}
}
