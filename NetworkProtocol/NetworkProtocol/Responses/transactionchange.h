#ifndef TRANSACTIONCHANGE_H
#define TRANSACTIONCHANGE_H

#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT TransactionChange : public Response
{
    qint32 m_stockId;
    qint32 m_amount;
    qint32 m_price;
    QString m_date;

public:
    TransactionChange(qint32, qint32, qint32, QString);

    qint16 length() const;
    void send(QIODevice *connection);
    Message::MessageType type() const;
};

}
}
#endif // TRANSACTIONCHANGE_H
