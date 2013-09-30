#ifndef COMPANYFROZEN_H
#define COMPANYFROZEN_H


#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT CompanyFrozenResp : public Response
{
    qint32 m_companyId;

public:
    CompanyFrozenResp(qint32 companyId);

    qint16 length() const;
    Message::MessageType type() const;
    void send(QIODevice *connection);
};

}
}
#endif // COMPANYFROZEN_H
