#include "companyactivemsg.h"

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

CompanyActive::CompanyActive(Types::CompanyIdType companyId)
{
    _companyId = companyId;
}

Types::Message::MessageType CompanyActive::type() const
{
    return Types::Message::MessageType::RESPONSE_COMPANY_ACTIVE;
}

void CompanyActive::send(QIODevice *connection)
{
    QDataStream out(connection);

    sendHeader(out);
    out << _companyId;
}

Types::Message::MessageLengthType CompanyActive::length() const
{
    return sizeof(Types::Message::MessageType) +
           sizeof(_companyId);
}


}
}
