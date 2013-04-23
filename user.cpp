#include "user.h"

#include "offer.h"
#include "transaction.h"

#include <QSqlQuery>
#include <QString>
#include <QVariant>

User::User(qint32 userId, double cash) : m_userId(userId), m_cash(cash)
{}

User::User(qint32 userId, QSqlDatabase& db) : m_userId(userId)
{
    //zapytanie powinno zwracaæ dok³¹dnie 1 rekord z 1 polem zawierajacym stan konta usera
    QString queryString;

    QSqlQuery query(db);
    query.setForwardOnly(true);

    db.transaction();

    query.exec(queryString);

    db.commit();

    m_cash = query.value(0).toDouble();
}

QVector<Offer> User::sellOffers(QSqlDatabase& db)
{
    //zapytanie powinno zwrocic wszystkie oferty sprzedazy tego uzytkownika
    //kazdy rekord powinien zawierac id_oferty, id_akcji, ilosc akcji, cene
    QString queryString;

    QSqlQuery query(db);
    query.setForwardOnly(true);

    db.transaction();

    query.exec(queryString);

    db.commit();

    QVector<Offer> ret;

    while (query.next())
    {
        Offer tmpOffer(query.value(0).toInt(), m_userId, query.value(1).toInt(),
                    query.value(2).toInt(), query.value(3).toDouble());


        ret.push_back(tmpOffer);
    }
    return ret;
}

QVector<Offer> User::buyOffers(QSqlDatabase& db)
{
    //zapytanie powinno zwrocic wszystkie oferty kupna tego uzytkownika
    //kazdy rekord powinien zawierac id_oferty, id_akcji, ilosc akcji, cene
    QString queryString;

    QSqlQuery query(db);
    query.setForwardOnly(true);

    db.transaction();

    query.exec(queryString);

    db.commit();

    QVector<Offer> ret;

    while (query.next())
    {
        Offer tmpOffer(query.value(0).toInt(), m_userId, query.value(1).toInt(),
                    query.value(2).toInt(), query.value(3).toDouble());


        ret.push_back(tmpOffer);
    }
    return ret;
}

QVector<Transaction> User::transactions(QSqlDatabase& db)
{
    //zapytanie powinno zwrocic wszystkie trnasakcje tego uzytkownika
    //kazdy rekord powinien zawierac id_transakcji, id_sprzedajacego, id_kupujacego,
    //id_akcji, ilosc akcji, cene
    QString queryString;

    QSqlQuery query(db);
    query.setForwardOnly(true);

    db.transaction();

    query.exec(queryString);

    db.commit();

    QVector<Transaction> ret;

    while (query.next())
    {
        Transaction tmpTran(query.value(0).toInt(), query.value(1).toInt(), query.value(2).toInt(),
                    query.value(3).toInt(), query.value(4).toInt(), query.value(5).toDouble());


        ret.push_back(tmpTran);
    }
    return ret;
}
