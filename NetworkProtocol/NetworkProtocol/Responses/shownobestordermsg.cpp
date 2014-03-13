#include "shownobestordermsg.h"

#include <utilities.h>
#include <stdexcept>

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

ShowNoBestOrder::ShowNoBestOrder(Types::Order::OrderType orderType,
                                 Types::StockIdType stockId)
    : ShowNoBestOrder(std::move(GlobalUtilities::getLogger()), orderType, stockId)
{}

ShowNoBestOrder::ShowNoBestOrder(std::shared_ptr<AbstractLogger> logger,
                                 Types::Order::OrderType orderType,
                                 Types::StockIdType stockId)
    : _orderType(orderType), _stockId(stockId)
{
    if(_stockId <= 0 )
    {
        LOG_TRACE(logger,
                  QString("Invalid argument: stockId(%1) <= 0")
                  .arg(_stockId.value));

        throw std::invalid_argument("stockId <= 0.");
    }
}

Types::Message::MessageType ShowNoBestOrder::type() const
{
    return Types::Message::MessageType::RESPONSE_SHOW_NO_BEST_ORDER;
}


Types::Message::MessageLengthType ShowNoBestOrder::length() const
{
    return Response::length() + sizeof(_orderType)+ sizeof(_stockId);
}

void ShowNoBestOrder::send(QIODevice *connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);

    out << _orderType << _stockId;
}

}
}
