#ifndef STOCK_H
#define STOCK_H

#include "offer.h"

#include <QSqlDatabase>
#include <QDataStream>
#include <QVector>


class Stock
{
    qint32 m_stockId;
    Offer m_bestSellOffer;
    Offer m_bestBuyOffer;

    Offer bestSellOffer(QSqlDatabase& db);
    Offer bestBuyOffer(QSqlDatabase& db);
public:
    Stock();
    Stock(qint32 stockId, Offer bestSellOffer, Offer bestBuyOffer);
    Stock(qint32 stockId, QSqlDatabase& db);

    Offer bestSellOffer();
    Offer bestBuyOffer();
    QVector<Offer> allSellOffer(QSqlDatabase& db);
    QVector<Offer> allBuyOffer(QSqlDatabase& db);

    friend QDataStream &operator<<(QDataStream& stream, const Stock& stock);
};

#endif // STOCK_H
