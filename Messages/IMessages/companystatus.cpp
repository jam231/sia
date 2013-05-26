#include "companystatus.h"

#include <QDataStream>

CompanyStatusMsg::CompanyStatusMsg(QDataStream& in)
{
    if(in.device()->bytesAvailable() < sizeof(m_companyId))
        throw InvalidDataInMsg();
    in >> m_companyId;
}


IOMessage::MessageType CompanyStatusMsg::type() const
{
    return COMPANY_STATUS_REQ;
}

qint32 CompanyStatusMsg::getCompanyId() const
{
    return m_companyId;
}
