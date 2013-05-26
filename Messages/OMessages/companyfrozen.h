#ifndef COMPANYFROZEN_H
#define COMPANYFROZEN_H


#include "omessage.h"



class CompanyFrozenResp : public OMessage
{
    qint32 m_companyId;
public:
    CompanyFrozenResp(qint32 companyId);

    IOMessage::MessageType type() const;
    void send(QIODevice *connection);
};

#endif // COMPANYFROZEN_H
