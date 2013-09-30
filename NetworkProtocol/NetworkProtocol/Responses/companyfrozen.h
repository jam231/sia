#ifndef COMPANYFROZEN_H
#define COMPANYFROZEN_H


#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT CompanyFrozenResp : public Response
{
    DTO::Types::CompanyIdType m_companyId;

public:
    CompanyFrozenResp(DTO::Types::CompanyIdType companyId);

    DTO::Types::MessageLengthType length() const;
    DTO::Types::MessageType type() const;
    void send(QIODevice *connection);
};

}
}
#endif // COMPANYFROZEN_H
