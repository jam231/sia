#ifndef LOGINUSERRESPFAIL_H
#define LOGINUSERRESPFAIL_H


#include "omessage.h"

#include <QString>

namespace NetworkProtocol {

class  LoginUserRespFail : public OMessage
{
    QString m_reason;

    qint16 length() const;
public:
    LoginUserRespFail(QString);

    MessageType type() const;
    void send(QIODevice* connection);
};

}
#endif // LOGINUSERRESPFAIL_H
