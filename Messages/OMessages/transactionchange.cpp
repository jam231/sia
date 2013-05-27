#include "transactionchange.h"
#include <QDataStream>

TransactionChange::TransactionChange(qint32 stockId, qint32 amount,
                                     qint32 price, QString date)
{
    m_stockId = stockId;
    m_amount = amount;
    m_price = price;
    m_date = date;
}

qint32 TransactionChange::length() const
{
    return -1;
}

void TransactionChange::send(QIODevice *connection)
{
    // Domyślnie BigEndian.
    QDataStream out(connection);

    auto date = m_date.toUtf8();
    out << static_cast<qint16>(sizeof(qint8) + 3*sizeof(qint32) + date.size())
        << static_cast<qint8>(type())
        << static_cast<qint32>(m_stockId)
        << static_cast<qint32>(m_amount)
        << static_cast<qint32>(m_price)
        << static_cast<qint16>(date.size());
    connection->write(date);
}

IOMessage::MessageType TransactionChange::type() const
{
    return TRANSACTION_CHANGE;
}
