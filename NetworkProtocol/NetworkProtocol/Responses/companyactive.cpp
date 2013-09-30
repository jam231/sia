#include "companyactive.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

CompanyActiveResp::CompanyActiveResp(Types::CompanyIdType companyId)
{
    m_companyId = companyId;
}

Types::MessageType CompanyActiveResp::type() const
{
    return Types::MessageType::COMPANY_ACTIVE_RESP;
}

void CompanyActiveResp::send(QIODevice *connection)
{
    QDataStream out(connection);

    sendHeader(out);
    out << m_companyId;
}

Types::MessageLengthType CompanyActiveResp::length() const
{
    return sizeof(Types::MessageType) +
           sizeof(m_companyId);
}

const QString CompanyActiveResp::getMessageName()
{
    return Response::getMessageName() + " :: CompanyActiveResp";
}

}
}