#include "companystatus.h"


namespace NetworkProtocol
{
namespace Requests
{

CompanyStatusMsg::CompanyStatusMsg(QDataStream& in) : Request(in)
{
    in >> m_companyId;
}


Message::MessageType CompanyStatusMsg::type() const
{
    return COMPANY_STATUS_REQ;
}

qint32 CompanyStatusMsg::getCompanyId() const
{
    return m_companyId;
}

qint16 CompanyStatusMsg::length() const
{
    return sizeof(MessageType) + sizeof(m_companyId);
}

}
}
