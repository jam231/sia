#include "market.h"

#include "transaction.h"
#include "Stock.h"

#include "registeruserrespmsg.h"
#include "buystockrespmsg.h"
#include "sellstockrespmsg.h"
#include "listofstocksmsg.h"
#include "changepricemsg.h"

#include <QSqlQuery>
#include <QVariant>

const int Market::SERVER_PORT = 6666;
const QString Market::DATABASE_USERNAME = "?";
const QString Market::DATABASE_PASSWORD = "?";
const QString Market::DATABASE_NAME = "?";
const QString Market::DATABASE_HOST = "localhost"; //?
const int Market::DATABASE_PORT = 80; //?

Market::Market(QObject* parent)
    : QObject(parent), m_server(new Server(this, SERVER_PORT)),
      m_database(QSqlDatabase::addDatabase("QPSQL"))
{
    m_database.setHostName(DATABASE_HOST);
    m_database.setDatabaseName(DATABASE_NAME);
    m_database.setUserName(DATABASE_USERNAME);
    m_database.setPassword(DATABASE_PASSWORD);
    m_database.setPort(DATABASE_PORT);

    m_database.open();
    connect(m_server, SIGNAL(registerUserReq(qint32, double)),
            this, SLOT(registerUserReq(qint32, double)) );
    connect(m_server, SIGNAL(subscribeStock(qint32, qint32)),
            this, SLOT(subscribeStock(qint32, qint32)) );
    connect(m_server, SIGNAL(unsubscribeStock(qint32, qint32)),
            this, SLOT(unsubscribeStock(qint32, qint32)) );
    connect(m_server, SIGNAL(sellStock(qint32, Offer)),
            this, SLOT(sellStock(qint32, Offer)) );
    connect(m_server, SIGNAL(buyStock(qint32, Offer)),
            this, SLOT(buyStock(qint32, Offer)) );
    connect(m_server, SIGNAL(getStocks(qint32)),this, SLOT(getStocks(qint32)) );
}

void Market::registerUserReq(qint32 tmpId, double cash)
{
    //zapytanie rejestrujące użytkownika powinno zwracać dokłądnie 1 rekord z nowym id
    QString queryString;

    QSqlQuery query(m_database);
    query.setForwardOnly(true);

    m_database.transaction();

    query.exec(queryString);

    m_database.commit();

    RegisterUserRespMsg Msg(query.value(0).toInt());

    m_server->send(Msg, tmpId);
}


void Market::subscribeStock(qint32 userId, qint32 stockId)
{
    //zapytanie nie powinno nic zwracac
    QString queryString;

    QSqlQuery query(m_database);
    query.setForwardOnly(true);

    m_database.transaction();

    query.exec(queryString);

    m_database.commit();
}

void Market::unsubscribeStock(qint32 userId, qint32 stockId)
{
    //zapytanie nie powinno nic zwracac
    QString queryString;

    QSqlQuery query(m_database);
    query.setForwardOnly(true);

    m_database.transaction();

    query.exec(queryString);

    m_database.commit();
}

void Market::sellStock(qint32 userId, Offer offer)
{
    //zapytanie powinno zwrócić tyle transakcji do ilu doszło w wyniku do dania tej oferty
    //każdą w osobnym rekordzie, każdy rekord powinien zawierać
    //id_transakcji,
    //id_usera sprzedajacego, id_usera kupujacego, id_akcji, ilosc_akcji, cene za akcje
    //w dokladnie takiej kolejnosci
    QString queryString;

    QSqlQuery query(m_database);
    query.setForwardOnly(true);

    m_database.transaction();

    query.exec(queryString);

    m_database.commit();

    bool anyAction = false;
    while (query.next())
    {
        Transaction tmpTran(query.value(0).toInt(), query.value(1).toInt(), query.value(2).toInt(),
                    query.value(3).toInt(), query.value(4).toInt(), query.value(5).toDouble());


        BuyStockRespMsg Msg1(tmpTran);
        SellStockRespMsg Msg2(tmpTran);

        m_server->send(Msg1, tmpTran.sellerId());
        m_server->send(Msg2, tmpTran.buyerId());

        anyAction = true;
    }

    if(anyAction)
        noticeChangePrice(offer.stockId());

}

void Market::buyStock(qint32 userId, Offer offer)
{
    //zapytanie powinno zwrócić tyle transakcji do ilu doszło w wyniku do dania tej oferty
    //każdą w osobnym rekordzie, każdy rekord powinien zawierać
    //id_transakcji(jesli bedziemy je wszystkie przechowywac w bazie),
    //id_usera sprzedajacego, id_usera kupujacego, id_akcji, ilosc_akcji, cene za akcje
    //w dokladnie takiej kolejnosci
    QString queryString;

    QSqlQuery query(m_database);
    query.setForwardOnly(true);

    m_database.transaction();

    query.exec(queryString);

    m_database.commit();

    bool anyAction = false;
    while (query.next())
    {
        Transaction tmpTran(query.value(0).toInt(), query.value(1).toInt(), query.value(2).toInt(),
                    query.value(3).toInt(), query.value(4).toInt(), query.value(5).toDouble());


        BuyStockRespMsg Msg1(tmpTran);
        SellStockRespMsg Msg2(tmpTran);

        m_server->send(Msg1, tmpTran.sellerId());
        m_server->send(Msg2, tmpTran.buyerId());

        anyAction = true;
    }

    if(anyAction)
        noticeChangePrice(offer.stockId());

}

void Market::getStocks(qint32 userId)
{
    //zapytanie powinno zwrocic tyle rekordow ile mamy akcji
    //kazdy rekord powinien zawierac nastepujace dane
    //id_akcji, najlepsza oferta sprzedazy, najlesza oferta kupna
    //gdzie oferty skladaja sie z
    //id_oferty, id_sprzedajacego/kupujacego, id_akcji, ilosci akcji, ceny za akcje
    //w dokladnie tej kolejnosci
    QString queryString;

    QSqlQuery query(m_database);
    query.setForwardOnly(true);

    m_database.transaction();

    query.exec(queryString);

    m_database.commit();

    ListOfStocksMsg Msg;
    while (query.next())
    {
        Stock s(query.value(0).toInt(),
                Offer(query.value(1).toInt(), query.value(2).toInt(), query.value(3).toInt(),
                      query.value(4).toInt(),query.value(5).toDouble()),
                Offer(query.value(6).toInt(), query.value(7).toInt(), query.value(8).toInt(),
                      query.value(9).toInt(),query.value(10).toDouble()));

        Msg.addStock(s);
    }

    m_server->send(Msg, userId);
}

void Market::noticeChangePrice(qint32 stockId)
{
    //zapytanie powinno zwracać id wszystkich uzytkownikow ktorzy zapisali sie na subskrypcje danej akcji
    QString queryString;

    QSqlQuery query(m_database);
    query.setForwardOnly(true);

    m_database.transaction();

    query.exec(queryString);

    m_database.commit();

    Stock s(stockId, m_database);

    while (query.next())
    {
        ChangePriceMsg Msg(s);

        m_server->send(Msg, query.value(0).toInt());
    }
}
