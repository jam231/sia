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

    sendHeader(out);
    out << m_companyId;
}

qint16 CompanyActiveResp::length() const
{
    return sizeof(MessageType) + sizeof(qint32);
}
