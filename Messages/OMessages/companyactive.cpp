#include "companyactive.h"

#include <QDataStream>

CompanyActiveResp::CompanyActiveResp(qint32 companyId)
{
    m_companyId = companyId;
}

IOMessage::MessageType CompanyActiveResp::type() const
{
    return COMPANY_ACTIVE_RESP;
}

void CompanyActiveResp::send(QIODevice *connection)
{
    QDataStream out(connection);
    out << static_cast<qint16>(sizeof(qint8) + sizeof(qint32))
        << static_cast<qint8>(type())
        << m_companyId;
}

qint16 CompanyActiveResp::length() const
{
    return -1;
}
