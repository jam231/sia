#ifndef GETSTOCKINFORESPMSG_H
#define GETSTOCKINFORESPMSG_H

#include "omessage.h"

#include <QPair>

namespace NetworkProtocol {

class  GetStockInfoRespMsg : public OMessage
{
    QPair<qint32, qint32> m_bestBuyOrder;
    QPair<qint32, qint32> m_bestSellOrder;
    QPair<QString, QPair<qint32, qint32> > m_lastTransaction;
    qint32 m_stockId;

    qint16 length() const;
public:
    GetStockInfoRespMsg(qint32 stockId,
                        QPair<qint32, qint32> bestBuyOrder    = qMakePair(0,0),\
                        QPair<qint32, qint32> bestSellOrder   = qMakePair(0,0),\
                        QPair<QString, QPair<qint32, qint32> > lastTransaction = qMakePair(QString(""), qMakePair(0,0)) );

    MessageType type() const;
    void send(QIODevice* connection);
};

}
#endif // GETSTOCKINFORESPMSG_H
