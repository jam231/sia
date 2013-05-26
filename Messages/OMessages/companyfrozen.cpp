#include "companyfrozen.h"

#include <QDataStream>

CompanyFrozenResp::CompanyFrozenResp(qint32 companyId)
{
    m_companyId = companyId;
}

IOMessage::MessageType CompanyFrozenResp::type() const
{
    return COMPANY_FROZEN_RESP;
}

void CompanyFrozenResp::send(QIODevice *connection)
{
    QDataStream out(connection);
    out << static_cast<qint16>(sizeof(qint8) + sizeof(qint32))
        << static_cast<qint8>(type())
        << m_companyId;
}
