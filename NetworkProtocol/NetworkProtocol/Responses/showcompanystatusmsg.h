#ifndef COMPANYSTATUSMSG_H
#define COMPANYSTATUSMSG_H

#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{
/*
 *  Responses :: ShowCompanyStatus
 *
 *  This message is for sending information about status of company with companyId.
 *
 *  Message format:
 *  <message_length : 2><message_type : 1><company_id : 4><company_status : 1>
 *
 *  where
 *      message_type         = MessageType::RESPONSE_SHOW_COMPANY_STATUS;
 */
class NETWORKPROTOCOLSHARED_EXPORT ShowCompanyStatus : public Response
{
        DTO::Types::CompanyIdType _companyId;
        DTO::Types::Company::CompanyStatusType _status;
public:
    ShowCompanyStatus(DTO::Types::CompanyIdType companyId,
                  DTO::Types::Company::CompanyStatusType status);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

    void send(QIODevice *connection);
};

}
}
#endif // COMPANYSTATUSMSG_H
