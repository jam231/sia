#ifndef GETSTOCKSMSG_H
#define GETSTOCKSMSG_H

#include "imessage.h"

class GetStocksMsg : public IMessage
{
public:
    GetStocksMsg();

    MessageType type() const;
};

#endif // GETSTOCKSMSG_H
