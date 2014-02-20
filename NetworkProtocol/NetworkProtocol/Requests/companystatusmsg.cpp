#include "companystatusmsg.h"


namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

CompanyStatus::CompanyStatus(QDataStream& in) : Request(in)
{
    in >> m_companyId;
}


Types::Message::MessageType CompanyStatus::type() const
{
    return Types::Message::MessageType::REQUEST_COMPANY_STATUS;
}

Types::CompanyIdType CompanyStatus::getCompanyId() const
{
    return m_companyId;
}

Types::Message::MessageLengthType CompanyStatus::length() const
{
    return sizeof(Types::Message::MessageType) +
           sizeof(m_companyId);
}

const QString CompanyStatus::getMessageName()
{
    return Request::getMessageName() + " :: CompanyStatus";
}

}
}
