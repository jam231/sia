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
    : CompanyStatus(std::move(GlobalUtilities::getLogger()), companyId, status)
{}

CompanyStatus::CompanyStatus(std::shared_ptr<AbstractLogger> logger,
                             Types::CompanyIdType companyId,
                             Types::Company::CompanyStatusType status)
    : _companyId(companyId), _status(status)
{
    if(_companyId <= 0)
    {
        LOG_TRACE(logger,
                  QString("Invalid argument: companyId(%1) <= 0")
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
    return  Response::length() +
            sizeof(_companyId) + sizeof(_status);
}


}
}
