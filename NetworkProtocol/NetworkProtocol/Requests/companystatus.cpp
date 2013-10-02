#include "companystatus.h"


namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

CompanyStatusMsg::CompanyStatusMsg(QDataStream& in) : Request(in)
{
    in >> m_companyId;
}


Types::Message::MessageType CompanyStatusMsg::type() const
{
    return Types::Message::MessageType::COMPANY_STATUS_REQ;
}

Types::CompanyIdType CompanyStatusMsg::getCompanyId() const
{
    return m_companyId;
}

Types::MessageLengthType CompanyStatusMsg::length() const
{
    return sizeof(Types::Message::MessageType) +
           sizeof(m_companyId);
}

const QString CompanyStatusMsg::getMessageName()
{
    return Request::getMessageName() + " :: CompanyStatusMsg";
}

}
}
