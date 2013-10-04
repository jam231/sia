#ifndef BESTORDERMSG_H
#define BESTORDERMSG_H

#include "response.h"
#include "DataTransferObjects/bestorder.h"

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT ShowBestOrder : public Response
{
    DTO::BestOrder m_bestOrder;
public:
    ShowBestOrder(DTO::Types::Order::OrderType, DTO::Types::StockIdType,
                  DTO::Types::AmountType, DTO::Types::PriceType);
    ShowBestOrder(DTO::BestOrder);

    DTO::Types::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

    static const QString getMessageName();

    void send(QIODevice *connection);
};

}
}
#endif // BESTORDERMSG_H
