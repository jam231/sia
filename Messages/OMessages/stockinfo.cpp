#include "stockinfo.h"
#include <QDataStream>

StockInfo::StockInfo(qint32 stockId, qint32 amount)
{
    m_stockId   = stockId;
    m_amount    = amount;
}

IOMessage::MessageType StockInfo::type() const
{
    return STOCK_INFO;
}


qint32 StockInfo::length() const
{
    return sizeof(type()) + sizeof(m_stockId) + sizeof(m_amount);
}

void StockInfo::send(QIODevice *connection)
{
    QDataStream out(connection);

    out << static_cast<qint16>(sizeof(qint8)+2*sizeof(qint32))
        << static_cast<qint8>(type())
        << m_stockId
        << m_amount;
}
