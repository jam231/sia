#include "companyfrozen.h"

#include <QDataStream>

CompanyFrozenResp::CompanyFrozenResp(qint32 companyId) : OMessage()
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

    sendHeader(out);
    out << m_companyId;
}

qint16 CompanyFrozenResp::length() const
{
    return sizeof(MessageType) + sizeof(qint32);
}
