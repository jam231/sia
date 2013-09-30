#include "companyfrozen.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

CompanyFrozenResp::CompanyFrozenResp(Types::CompanyIdType companyId)
{
    m_companyId = companyId;
}

Message::MessageType CompanyFrozenResp::type() const
{
    return COMPANY_FROZEN_RESP;
}

void CompanyFrozenResp::send(QIODevice *connection)
{
    QDataStream out(connection);

    sendHeader(out);
    out << m_companyId;
}

Types::MessageLengthType CompanyFrozenResp::length() const
{
    return sizeof(MessageType) + sizeof(m_companyId);
}

}
}
