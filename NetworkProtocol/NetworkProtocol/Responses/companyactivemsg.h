#ifndef COMPANYACTIVEMSG_H
#define COMPANYACTIVEMSG_H

#include "response.h"

namespace NetworkProtocol
{
namespace Responses
{

class NETWORKPROTOCOLSHARED_EXPORT CompanyActive : public Response
{
    DTO::Types::CompanyIdType m_companyId;
public:
    CompanyActive(DTO::Types::CompanyIdType companyId);

    DTO::Types::Message::MessageLengthType length() const;
    DTO::Types::Message::MessageType type() const;


    void send(QIODevice *connection);
};

}
}
#endif // COMPANYACTIVEMSG_H
