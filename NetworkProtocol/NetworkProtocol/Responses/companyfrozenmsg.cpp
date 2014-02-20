#include "companyfrozenmsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

CompanyFrozen::CompanyFrozen(Types::CompanyIdType companyId)
{
    m_companyId = companyId;
}

Types::Message::MessageType CompanyFrozen::type() const
{
    return Types::Message::MessageType::RESPONSE_COMPANY_FROZEN;
}

void CompanyFrozen::send(QIODevice *connection)
{
    QDataStream out(connection);

    sendHeader(out);
    out << m_companyId;
}

Types::Message::MessageLengthType CompanyFrozen::length() const
{
    return sizeof(Types::Message::MessageType) +
           sizeof(m_companyId);
}

const QString CompanyFrozen::getMessageName()
{
    return Response::getMessageName() + " :: CompanyFrozen";
}

}
}
