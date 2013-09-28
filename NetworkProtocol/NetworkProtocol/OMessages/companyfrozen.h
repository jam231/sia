#ifndef COMPANYFROZEN_H
#define COMPANYFROZEN_H


#include "omessage.h"

namespace NetworkProtocol {

class NETWORKPROTOCOLSHARED_EXPORT CompanyFrozenResp : public OMessage
{
    qint32 m_companyId;

    qint16 length() const;
public:
    CompanyFrozenResp(qint32 companyId);

    IOMessage::MessageType type() const;
    void send(QIODevice *connection);
};

}
#endif // COMPANYFROZEN_H
