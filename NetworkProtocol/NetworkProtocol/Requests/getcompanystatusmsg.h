#ifndef COMPANYSTATUSMSG_H
#define COMPANYSTATUSMSG_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
/*
 *  Request :: GetCompanyStatus
 *
 *  This message is for requesting company status.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1><company_id : 4>
 *
 *  where
 *      message_type         = MessageType::REQUEST_GET_COMPANY_STATUS (0x28)
 */
class NETWORKPROTOCOLSHARED_EXPORT GetCompanyStatus : public Request
{
    DTO::Types::CompanyIdType _companyId;

public:
    GetCompanyStatus(QDataStream&);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;
    DTO::Types::CompanyIdType getCompanyId() const;

};
}
}
#endif // COMPANYSTATUSMSG_H
