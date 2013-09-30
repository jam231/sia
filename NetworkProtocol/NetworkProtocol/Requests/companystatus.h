#ifndef COMPANYSTATUS_H
#define COMPANYSTATUS_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
class NETWORKPROTOCOLSHARED_EXPORT CompanyStatusMsg : public Request
{
    DTO::Types::CompanyIdType m_companyId;

public:
    CompanyStatusMsg(QDataStream& in);

    DTO::Types::MessageLengthType length() const;
    DTO::Types::MessageType type() const;
    DTO::Types::CompanyIdType getCompanyId() const;
};
}
}
#endif // COMPANYSTATUS_H
