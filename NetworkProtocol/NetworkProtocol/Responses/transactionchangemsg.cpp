#include "transactionchangemsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

TransactionChange::TransactionChange(Types::StockIdType stockId, Types::AmountType amount,
                                     Types::PriceType price, QString dateTime)
{
    m_stockId = stockId;
    m_amount = amount;
    m_price = price;
    m_dateTime = dateTime;
}

Types::Message::MessageLengthType TransactionChange::length() const
{
    return sizeof(Types::Message::MessageType) +
            sizeof(m_stockId) + sizeof(m_amount) + sizeof(m_price) + m_dateTime.toUtf8().size();
}

void TransactionChange::send(QIODevice *connection)
{
    // Domyślnie BigEndian.
    QDataStream out(connection);

    QByteArray dateTime_bytes = m_dateTime.toUtf8();

    sendHeader(out);
    out << m_stockId
        << m_amount
        << m_price
        << static_cast<Types::Message::MessageLengthType>(dateTime_bytes.size());
    connection->write(dateTime_bytes);
}

Types::Message::MessageType TransactionChange::type() const
{
    return Types::Message::MessageType::RESPONSE_TRANSACTION_CHANGE;
}

const QString TransactionChange::getMessageName()
{
    return Response::getMessageName() + " :: TransactionChange";
}

}
}
