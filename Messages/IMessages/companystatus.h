#ifndef COMPANYSTATUS_H
#define COMPANYSTATUS_H

#include "imessage.h"

class CompanyStatusMsg : public IMessage
{
    qint32 m_companyId;

    qint16 length() const;
public:
    CompanyStatusMsg(QDataStream& in);
    MessageType type() const;
    qint32 getCompanyId() const;
};

#endif // COMPANYSTATUS_H
