#include "lasttransaction.h"

namespace NetworkProtocol
{
namespace DTO
{

LastTransaction::LastTransaction()
    : m_dateTime(""), m_amount(0), m_price(0)
{
}

QString LastTransaction::getDateTime() const
{
    return m_dateTime;
}

Types::AmountType LastTransaction::getAmount() const
{
    return m_amount;
}

Types::PriceType LastTransaction::getPrice() const
{
    return m_price;
}

Types::MessageLengthType LastTransaction::lengthInBytes() const
{
    return m_dateTime.toUtf8().size() + sizeof(m_amount) + sizeof(m_price);
}

LastTransaction::LastTransaction(QString dateTime, Types::AmountType amount, Types::PriceType price)
    : m_dateTime(dateTime), m_amount(amount), m_price(price)
{
}

}
}

