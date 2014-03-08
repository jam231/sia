#include "showbestordermsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

ShowBestOrder::ShowBestOrder(Types::Order::OrderType type, Types::StockIdType stockId,
                             Types::AmountType amount, Types::PriceType price)
    : ShowBestOrder(std::move(GlobalUtilities::getLogger()), type, stockId,
                    amount, price)
{}

ShowBestOrder::ShowBestOrder(std::shared_ptr<AbstractLogger> logger,
                             Types::Order::OrderType type, Types::StockIdType stockId,
                             Types::AmountType amount, Types::PriceType price)
    : _bestOrder(BestOrder(std::move(logger), type, stockId, amount, price))
{
    // No need to check the validity of the arguments -
    // ShowBestOrder constructor delegates it to
    // constructor of BestOrder.
}

ShowBestOrder::ShowBestOrder(BestOrder bestOrder)
    : _bestOrder(bestOrder)
{
}

Types::Message::MessageType ShowBestOrder::type() const
{
    return Types::Message::MessageType::RESPONSE_SHOW_BEST_ORDER;
}


Types::Message::MessageLengthType ShowBestOrder::length() const
{
    return Response::length() + _bestOrder.lengthSerialized();
}

void ShowBestOrder::send(QIODevice *connection)
{
    // Domyślnie BigEndian
    QDataStream out(connection);

    sendHeader(out);

    out << _bestOrder;
}

}
}
