#ifndef REGISTERUSERRESPFAIL_H
#define REGISTERUSERRESPFAIL_H

#include "omessage.h"
#include <QString>

class RegisterUserRespFail : public OMessage
{
    QString m_reason;

    qint32 length() const;
public:
    RegisterUserRespFail(QString reason);

    MessageType type() const;
    void send(QIODevice* connection);
};


#endif // REGISTERUSERRESPFAIL_H
