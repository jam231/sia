#include "showbestordermsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

ShowBestOrder::ShowBestOrder(Types::Order::OrderType type, Types::StockIdType stockId,
                     Types::AmountType amount, Types::PriceType price)
    : _bestOrder(BestOrder(type, stockId, amount, price))
{
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
    return sizeof(Types::Message::MessageType) + _bestOrder.lengthSerialized();
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
