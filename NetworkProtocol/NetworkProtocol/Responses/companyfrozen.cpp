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

Types::Message::MessageType CompanyFrozenResp::type() const
{
    return Types::Message::MessageType::COMPANY_FROZEN_RESP;
}

void CompanyFrozenResp::send(QIODevice *connection)
{
    QDataStream out(connection);

    sendHeader(out);
    out << m_companyId;
}

Types::MessageLengthType CompanyFrozenResp::length() const
{
    return sizeof(Types::Message::MessageType) +
           sizeof(m_companyId);
}

const QString CompanyFrozenResp::getMessageName()
{
    return Response::getMessageName() + " :: CompanyFrozenResp";
}

}
}
