#ifndef GETMYSTOCKSRESPMSG_H
#define GETMYSTOCKSRESPMSG_H

#include <omessage.h>
#include <QVector>
#include <QPair>

class GetMyStocksRespMsg : public OMessage
{
    QVector<QPair<qint32, qint32> > m_stocks;

    qint16 length() const;
public:
    GetMyStocksRespMsg();

    MessageType type() const;
    void send(QIODevice* connection);

    void addStock(qint32 stockId, qint32 amount);
};

#endif // GETMYSTOCKSRESPMSG_H
