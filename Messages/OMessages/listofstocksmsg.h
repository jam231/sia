#ifndef LISTOFSTOCKSMSG_H
#define LISTOFSTOCKSMSG_H

#include "omessage.h"
#include "stock.h"

#include <QIODevice>
#include <QVector>


/*
 *  Nie działa
 *
 *      --jam231
 */


class ListOfStocksMsg : public OMessage
{
    QVector<Stock> m_stocks;

    qint32 length() const;
public:
    ListOfStocksMsg();
    ListOfStocksMsg(const QVector<Stock>& stocks);

    MessageType type() const;
    void send(QIODevice* connection);

    void addStock(const Stock& stock);
};

#endif // LISTOFSTOCKSMSG_H
