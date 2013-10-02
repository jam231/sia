#ifndef COMPANYACTIVE_H
#define COMPANYACTIVE_H

#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT CompanyActiveResp : public Response
{
    DTO::Types::CompanyIdType m_companyId;
public:
    CompanyActiveResp(DTO::Types::CompanyIdType companyId);

    DTO::Types::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

    static const QString getMessageName();

    void send(QIODevice *connection);
};

}
}
#endif // COMPANYACTIVE_H
