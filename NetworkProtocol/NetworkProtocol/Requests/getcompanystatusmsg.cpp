#include "getcompanystatusmsg.h"

#include "networkprotocol_utilities.h"

namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

GetCompanyStatus::GetCompanyStatus(QDataStream& serialized_request)
    : GetCompanyStatus(std::move(GlobalUtilities::getLogger()), serialized_request)
{}

GetCompanyStatus::GetCompanyStatus(std::shared_ptr<AbstractLogger> logger,
                                   QDataStream& serialized_request)
{
    if(serialized_request.device()->bytesAvailable() < sizeof(_companyId))
    {
         LOG_TRACE(logger,
                         QString("Malformed request: Not enough bytes in"\
                                 "serialized_request to company id. Is %1 should be >%2.")
                        .arg(serialized_request.device()->bytesAvailable())
                        .arg(sizeof(_companyId)));
        throw MalformedRequest("Not enough bytes in serialized_request to read"\
                               " company id.");
    }

    serialized_request >> _companyId;

    if(_companyId <= 0)
    {
        LOG_TRACE(logger, QString("Invalid company id. companyId(%1) <= 0.")
                          .arg(_companyId.value));
        throw InvalidRequestBody("company id <= 0.");
    }
}


Types::Message::MessageType GetCompanyStatus::type() const
{
    return Types::Message::MessageType::REQUEST_GET_COMPANY_STATUS;
}

Types::CompanyIdType GetCompanyStatus::getCompanyId() const
{
    return _companyId;
}

Types::Message::MessageLengthType GetCompanyStatus::length() const
{
    return Request::length() + sizeof(_companyId);
}


}
}
