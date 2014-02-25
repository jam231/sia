#ifndef SHOWUSERSTOCKSMSG_H
#define SHOWUSERSTOCKSMSG_H

#include "response.h"

#include <QVector>
#include <QPair>

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT ShowUserStocks : public Response
{
    QVector<QPair<DTO::Types::StockIdType, DTO::Types::AmountType> > m_stocks;

public:
    ShowUserStocks();

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

    void send(QIODevice* connection);

    void addStock(DTO::Types::StockIdType stockId, DTO::Types::AmountType amount);
};

}
}
#endif // SHOWUSERSTOCKSMSG_H
