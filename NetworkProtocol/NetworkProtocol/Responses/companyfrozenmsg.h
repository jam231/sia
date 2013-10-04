#ifndef COMPANYFROZEN_H
#define COMPANYFROZEN_H


#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT CompanyFrozen : public Response
{
    DTO::Types::CompanyIdType m_companyId;

public:
    CompanyFrozen(DTO::Types::CompanyIdType companyId);

    DTO::Types::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;

    static const QString getMessageName();

    void send(QIODevice *connection);
};

}
}
#endif // COMPANYFROZEN_H
