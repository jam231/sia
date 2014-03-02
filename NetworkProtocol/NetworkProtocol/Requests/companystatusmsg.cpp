#include "companystatusmsg.h"

#include "networkprotocol_utilities.h"

namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

CompanyStatus::CompanyStatus(QDataStream& serialized_request)
{
    if(serialized_request.device()->bytesAvailable() < sizeof(_companyId))
    {
        LOG_TRACE(QString("Malformed request: Not enough bytes in serialized_request"\
                          " to company id. Is %1 should be >%2.")
                  .arg(serialized_request.device()->bytesAvailable())
                  .arg(sizeof(_companyId)));
        throw MalformedRequest("Not enough bytes in serialized_request to read"\
                               " company id.");
    }

    serialized_request >> _companyId;

    if(_companyId <= 0)
    {
        LOG_TRACE(QString("Invalid company id. companyId(%1) <= 0.")
                  .arg(_companyId.value));
        throw InvalidRequestBody("company id <= 0.");
    }
}


Types::Message::MessageType CompanyStatus::type() const
{
    return Types::Message::MessageType::REQUEST_COMPANY_STATUS;
}

Types::CompanyIdType CompanyStatus::getCompanyId() const
{
    return _companyId;
}

Types::Message::MessageLengthType CompanyStatus::length() const
{
    return sizeof(_companyId);
}


}
}
