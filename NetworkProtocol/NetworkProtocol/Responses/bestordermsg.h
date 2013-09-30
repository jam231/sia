#ifndef BESTORDERMSG_H
#define BESTORDERMSG_H

#include "response.h"
#include "DataTransferObjects/bestorder.h"

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT BestOrderMsg : public Response
{
    DTO::BestOrder m_bestOrder;
public:
    BestOrderMsg(DTO::Types::OrderType, DTO::Types::StockIdType,
                 DTO::Types::AmountType, DTO::Types::PriceType);
    BestOrderMsg(DTO::BestOrder);

    DTO::Types::MessageLengthType length() const;
    DTO::Types::MessageType type() const;

    void send(QIODevice *connection);
};

}
}
#endif // BESTORDERMSG_H
