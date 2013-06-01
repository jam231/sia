#ifndef GETMYSTOCKSMSG_H
#define GETMYSTOCKSMSG_H

#include <imessage.h>

class GetMyStocksMsg : public IMessage
{
public:
    GetMyStocksMsg();

    MessageType type() const;
};

#endif // GETMYSTOCKSMSG_H
