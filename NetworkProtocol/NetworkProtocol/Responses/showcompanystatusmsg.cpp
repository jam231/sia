#include "showcompanystatusmsg.h"

#include <utilities.h>

#include <stdexcept>

namespace NetworkProtocol
{
namespace Responses
{

using namespace DTO;

ShowCompanyStatus::ShowCompanyStatus(Types::CompanyIdType companyId,
                             Types::Company::CompanyStatusType status)
    : _companyId(companyId), _status(status)
{
    if(_companyId <= 0)
    {
        LOG_TRACE(QString("Invalid argument: companyId(%1) <= 0")
                  .arg(_companyId.value));
        throw std::invalid_argument("companyId <= 0.");
    }
}

Types::Message::MessageType ShowCompanyStatus::type() const
{
    return Types::Message::MessageType::RESPONSE_SHOW_COMPANY_STATUS;
}

void ShowCompanyStatus::send(QIODevice *connection)
{
    QDataStream out(connection);

    sendHeader(out);
    out << _companyId
        << _status;
}

Types::Message::MessageLengthType ShowCompanyStatus::length() const
{
    return sizeof(Types::Message::MessageType) +
            sizeof(_companyId) + sizeof(_status);
}


}
}
