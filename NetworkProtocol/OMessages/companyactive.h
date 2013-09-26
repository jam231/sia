#ifndef COMPANYACTIVE_H
#define COMPANYACTIVE_H

#include "omessage.h"

namespace NetworkProtocol {

class NETWORKPROTOCOLSHARED_EXPORT CompanyActiveResp : public OMessage
{
    qint32 m_companyId;

    qint16 length() const;
public:
    CompanyActiveResp(qint32 companyId);
    void send(QIODevice *connection);
    IOMessage::MessageType type() const;
};

}

#endif // COMPANYACTIVE_H
