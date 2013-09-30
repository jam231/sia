#include "transactionchange.h"

namespace NetworkProtocol
{
namespace Responses
{

TransactionChange::TransactionChange(qint32 stockId, qint32 amount,
                                     qint32 price, QString date)
{
    m_stockId = stockId;
    m_amount = amount;
    m_price = price;
    m_date = date;
}

qint16 TransactionChange::length() const
{
    return sizeof(MessageType) + 3*sizeof(qint32) + sizeof(qint16) + m_date.size();
}

void TransactionChange::send(QIODevice *connection)
{
    // Domyślnie BigEndian.
    QDataStream out(connection);

    QByteArray date = m_date.toUtf8();

    sendHeader(out);
    out << static_cast<qint32>(m_stockId)
        << static_cast<qint32>(m_amount)
        << static_cast<qint32>(m_price)
        << static_cast<qint16>(date.size());
    connection->write(date);
}

Message::MessageType TransactionChange::type() const
{
    return TRANSACTION_CHANGE;
}

}
}
