#ifndef STOCKINFO_H
#define STOCKINFO_H

#include "omessage.h"

#include <QIODevice>

class StockInfo : public OMessage
{
/*    qint32 m_stockId;
    qint32 m_amount;
*/
    qint32 length() const;
public:
    qint32 m_stockId;
    qint32 m_amount;

    StockInfo(qint32 stockId, qint32 amount);

    MessageType type() const;
    void send(QIODevice* connection);
};

#endif // STOCKINFO_H
