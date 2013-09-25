#ifndef LOGINUSERRESPOK_H
#define LOGINUSERRESPOK_H


#include "omessage.h"

namespace NetworkProtocol {

class NETWORKPROTOCOLSHARED_EXPORT LoginUserRespOk : public OMessage
{

    qint16 length() const;
public:
    LoginUserRespOk();
    MessageType type() const;
    void send(QIODevice* connection);
};

}
#endif // LOGINUSERRESPOK_H
