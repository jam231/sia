#ifndef COMPANYACTIVE_H
#define COMPANYACTIVE_H

#include <omessage.h>

class CompanyActiveResp : public OMessage
{
    qint32 m_companyId;
public:
    CompanyActiveResp(qint32 companyId);
    void send(QIODevice *connection);
    IOMessage::MessageType type() const;
};

#endif // COMPANYACTIVE_H
