#ifndef COMPANYFROZEN_H
#define COMPANYFROZEN_H


#include "omessage.h"

#include <QIODevice>

class CompanyFrozenResp : public OMessage
{
    qint32 m_companyId;

    qint32 length() const;
public:
    CompanyFrozenResp(qint32 companyId);

    IOMessage::MessageType type() const;
    void send(QIODevice *connection);
};

#endif // COMPANYFROZEN_H
