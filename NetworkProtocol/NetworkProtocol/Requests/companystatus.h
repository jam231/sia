#ifndef COMPANYSTATUS_H
#define COMPANYSTATUS_H

#include "request.h"

namespace NetworkProtocol {
namespace Requests
{
class NETWORKPROTOCOLSHARED_EXPORT CompanyStatusMsg : public Request
{
    qint32 m_companyId;

public:
    CompanyStatusMsg(QDataStream& in);

    qint16 length() const;
    MessageType type() const;
    qint32 getCompanyId() const;
};
}
}
#endif // COMPANYSTATUS_H
