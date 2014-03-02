#ifndef COMPANYSTATUSMSG_H
#define COMPANYSTATUSMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
/*
 *  Request :: CompanyStatus
 *
 *  This message is for requesting company status.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1><company_id : 4>
 *
 *  where
 *      message_type         = MessageType::REQUEST_COMPANY_STATUS
 */
class NETWORKPROTOCOLSHARED_EXPORT CompanyStatus : public Request
{
    DTO::Types::CompanyIdType _companyId;

public:
    CompanyStatus(QDataStream&);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;
    DTO::Types::CompanyIdType getCompanyId() const;

};
}
}
#endif // COMPANYSTATUSMSG_H
