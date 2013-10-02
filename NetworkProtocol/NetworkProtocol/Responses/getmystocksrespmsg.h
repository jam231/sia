#ifndef GETMYSTOCKSRESPMSG_H
#define GETMYSTOCKSRESPMSG_H

#include "response.h"

#include <QVector>
#include <QPair>

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT GetMyStocksRespMsg : public Response
{
    QVector<QPair<DTO::Types::StockIdType, DTO::Types::AmountType> > m_stocks;

public:
    GetMyStocksRespMsg();

    DTO::Types::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

    static const QString getMessageName();

    void send(QIODevice* connection);

    void addStock(DTO::Types::StockIdType stockId, DTO::Types::AmountType amount);
};

}
}
#endif // GETMYSTOCKSRESPMSG_H
