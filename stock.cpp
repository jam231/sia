#include "stock.h"

#include "offer.h"

#include <QString>
#include <QSqlQuery>
#include <QVariant>

Stock::Stock() : m_stockId(-1), m_bestSellOffer(), m_bestBuyOffer()
{}

Stock::Stock(qint32 stockId, QSqlDatabase& db) : m_stockId(stockId)
{
    m_bestSellOffer = bestSellOffer(db);
    m_bestBuyOffer = bestBuyOffer(db);
}

Stock::Stock(qint32 stockId, Offer bestSellOffer, Offer bestBuyOffer)
    : m_stockId(stockId), m_bestSellOffer(bestSellOffer), m_bestBuyOffer(bestBuyOffer)
{}

Offer Stock::bestSellOffer(QSqlDatabase& db)
{
    //zapytanie powinno zwrocic 1 rekord zwierajacy najlepsza oferte sprzedazy
    //rekord powinien zawierac id_oferty, id_sprzedawcy/kupujacego, id_akcji, ilosc akcji, cene
    //jesli nie ma zadnej oferty wszsytkie pola powinny byc rowne -1
    QString queryString;

    QSqlQuery query(db);
    query.setForwardOnly(true);

    db.transaction();

    query.exec(queryString);

    db.commit();

    return Offer(query.value(0).toInt(), query.value(1).toInt(), query.value(2).toInt(),
                 query.value(3).toInt(), query.value(4).toDouble());
}

Offer Stock::bestBuyOffer(QSqlDatabase& db)
{
    //zapytanie powinno zwrocic 1 rekord zwierajacy najlepsza oferte kupna
    //rekord powinien zawierac id_oferty, id_sprzedawcy/kupujacego, id_akcji, ilosc akcji, cene
    //jesli nie ma zadnej oferty wszsytkie pola powinny byc rowne -1
    QString queryString;

    QSqlQuery query(db);
    query.setForwardOnly(true);

    db.transaction();

    query.exec(queryString);

    db.commit();

    return Offer(query.value(0).toInt(), query.value(1).toInt(), query.value(2).toInt(),
                 query.value(3).toInt(), query.value(4).toDouble());
}

Offer Stock::bestSellOffer()
{
    return m_bestSellOffer;
}

Offer Stock::bestBuyOffer()
{
    return m_bestBuyOffer;
}

QVector<Offer> Stock::allSellOffer(QSqlDatabase& db)
{
    //zapytanie powinno zwrocic rekordy zwierajacy wszsytkie oferty sprzedazy
    //1 rekord powinien zawierac id_oferty, id_sprzedawcy/kupujacego, id_akcji, ilosc akcji, cene
    QString queryString;

    QSqlQuery query(db);
    query.setForwardOnly(true);

    db.transaction();

    query.exec(queryString);

    db.commit();

    QVector<Offer> ret;

    while (query.next())
    {
        Offer tmpOffer(query.value(0).toInt(), query.value(1).toInt(), query.value(2).toInt(),
                    query.value(3).toInt(), query.value(4).toDouble());


        ret.push_back(tmpOffer);
    }
    return ret;
}

QVector<Offer> Stock::allBuyOffer(QSqlDatabase& db)
{
    //zapytanie powinno zwrocic rekordy zwierajacy wszsytkie oferty kupna
    //1 rekord powinien zawierac id_oferty, id_sprzedawcy/kupujacego, id_akcji, ilosc akcji, cene
    QString queryString;

    QSqlQuery query(db);
    query.setForwardOnly(true);

    db.transaction();

    query.exec(queryString);

    db.commit();

    QVector<Offer> ret;

    while (query.next())
    {
        Offer tmpOffer(query.value(0).toInt(), query.value(1).toInt(), query.value(2).toInt(),
                    query.value(3).toInt(), query.value(4).toDouble());


        ret.push_back(tmpOffer);
    }
    return ret;
}

QDataStream &operator<<(QDataStream& stream, const Stock& stock)
{
    stream<<stock.m_stockId<<stock.m_bestSellOffer<<stock.m_bestSellOffer;

    return stream;
}
