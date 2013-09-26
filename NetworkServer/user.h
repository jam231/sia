#ifndef USER_H
#define USER_H

class Offer;
class Transaction;

#include <QVector>
#include <QSqlDatabase>


class User
{
    qint32 m_userId;
    double m_cash;
public:
    User(qint32 userId, double cash);
    User(qint32 userId, QSqlDatabase& db);

    QVector<Offer> sellOffers(QSqlDatabase& db);
    QVector<Offer> buyOffers(QSqlDatabase& db);
    QVector<Transaction> transactions(QSqlDatabase& db);
    //
    //tu moga byc inne funkcje dotyczacych danych uzytkownika ktore trzeba wycignac z bazy danych
    //jak na przyklad jego transakcje
    //
};

#endif // USER_H
