#ifndef GETMYORDERMSG_H
#define GETMYORDERMSG_H

#include <imessage.h>

class GetMyOrderMsg : public IMessage
{
public:
    GetMyOrderMsg();

    MessageType type() const;
};

#endif // GETMYORDERMSG_H
