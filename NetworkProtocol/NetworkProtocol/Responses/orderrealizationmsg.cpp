#include "orderrealizationmsg.h"

#include <utilities.h>
#include <stdexcept>

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

OrderRealization::OrderRealization(Types::OrderIdType orderId,
                                   Types::AmountType amount)
    : OrderRealization(std::move(GlobalUtilities::getLogger()), orderId, amount)
{}

OrderRealization::OrderRealization(std::shared_ptr<AbstractLogger> logger,
                                   Types::OrderIdType orderId,
                                   Types::AmountType amount)
    : _orderId(orderId), _amount(amount)
{

    if(_orderId <= 0 || _amount <= 0)
    {
        LOG_TRACE(logger,
                  QString("Invalid arguments: orderId(%1) <= 0 || amount(%2) <= 0 ")
                  .arg(_orderId.value).arg(_amount.value));

        throw std::invalid_argument("One of orderId, amount is <= 0.");
    }
}

void OrderRealization::send(QIODevice *connection)
{
    QDataStream out(connection);

    sendHeader(out);
    out  << _orderId
         << _amount;
}

Types::Message::MessageType OrderRealization::type() const
{
    return Types::Message::MessageType::RESPONSE_ORDER_REALIZATION;
}

Types::Message::MessageLengthType OrderRealization::length() const
{
    return Response::length() + sizeof(_orderId) + sizeof(_amount);
}


}
}
