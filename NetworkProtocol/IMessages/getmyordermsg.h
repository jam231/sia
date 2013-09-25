#ifndef GETMYORDERMSG_H
#define GETMYORDERMSG_H

#include "imessage.h"

namespace NetworkProtocol {

class  GetMyOrderMsg : public IMessage
{
    qint16 length() const;
public:
    GetMyOrderMsg();

    MessageType type() const;
};

}
#endif // GETMYORDERMSG_H
