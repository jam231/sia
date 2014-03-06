#include "companystatusmsg.h"

#include <utilities.h>

#include <stdexcept>

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

CompanyStatus::CompanyStatus(Types::CompanyIdType companyId,
                             Types::Company::CompanyStatusType status)
    : _companyId(companyId), _status(status)
{
    if(_companyId <= 0)
    {
        GLOBAL_LOG_TRACE(QString("Invalid argument: companyId(%1) <= 0")
                  .arg(_companyId.value));
        throw std::invalid_argument("companyId <= 0.");
    }
}

Types::Message::MessageType CompanyStatus::type() const
{
    return Types::Message::MessageType::RESPONSE_COMPANY_STATUS;
}

void CompanyStatus::send(QIODevice *connection)
{
    QDataStream out(connection);

    sendHeader(out);
    out << _companyId
        << _status;
}

Types::Message::MessageLengthType CompanyStatus::length() const
{
    return sizeof(Types::Message::MessageType) +
            sizeof(_companyId) + sizeof(_status);
}


}
}
