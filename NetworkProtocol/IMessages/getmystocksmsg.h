#ifndef GETMYSTOCKSMSG_H
#define GETMYSTOCKSMSG_H

#include "imessage.h"

namespace NetworkProtocol {

class NETWORKPROTOCOLSHARED_EXPORT GetMyStocksMsg : public IMessage
{
    qint16 length() const;
public:
    GetMyStocksMsg();

    MessageType type() const;
};

}
#endif // GETMYSTOCKSMSG_H
