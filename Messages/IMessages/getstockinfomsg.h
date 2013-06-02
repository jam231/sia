#ifndef GETSTOCKINFOMSG_H
#define GETSTOCKINFOMSG_H

#include <imessage.h>
#include <QDataStream>

class GetStockInfoMsg : public IMessage
{
    qint32 m_stockId;
public:
    GetStockInfoMsg(QDataStream& in);

    MessageType type() const;
    qint32 getStockId() const;
};

#endif // GETSTOCKINFOMSG_H
