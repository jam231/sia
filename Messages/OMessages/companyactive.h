#ifndef COMPANYACTIVE_H
#define COMPANYACTIVE_H

#include "omessage.h"

#include <QIODevice>



class CompanyActiveResp : public OMessage
{
    qint32 m_companyId;

    qint32 length() const;
public:
    CompanyActiveResp(qint32 companyId);
    void send(QIODevice *connection);
    IOMessage::MessageType type() const;
};

#endif // COMPANYACTIVE_H
