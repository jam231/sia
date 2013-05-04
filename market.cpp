#include "market.h"

#include "transaction.h"
#include "stock.h"

#include "registeruserrespmsg.h"
#include "buystockrespmsg.h"
#include "sellstockrespmsg.h"
#include "listofstocksmsg.h"
#include "changepricemsg.h"

#include "configmanager.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>



Market::Market(const ConfigManager<>& config, QObject* parent)
    : QObject(parent)
{
    qDebug() << "[Market] Ładowanie danych konfiguracyjnych...";
    int serverPort = config.intAt("SERVER PORT");
    QString dbUserName = config["DATABASE USERNAME"];
    QString dbPassword = config["DATABASE PASSWORD"];
    QString dbName = config["DATABASE NAME"];
    QString dbHost = config["DATABASE HOST"];
    int dbPort = config.intAt("DATABASE PORT");

    m_database = QSqlDatabase::addDatabase("QPSQL",
                                           QLatin1String("Rynki finansowe 1"));

    m_database.setHostName(dbHost);
    m_database.setDatabaseName(dbName);
    m_database.setUserName(dbUserName);
    m_database.setPassword(dbPassword);
    m_database.setPort(dbPort);

    qDebug() << "[Market] Otwieranie nowego połączenia z bazą danych...";
    if(!m_database.open())
    {
        qDebug() << "[Market] Wykryto błąd" << m_database.lastError().text()
                 << "podczas otwierania nowego połączenia z bazą daynch.";
        throw DatabaseError();
    }
    qDebug() << "[Market] Ustanowiono połączenie z bazą danych.";

    //qDebug() << "Database tables:\n" << m_database.tables();

    qDebug() << "[Market] Uruchamianie serwera...";
    m_server = new Server(this, serverPort);

    connect(m_server, SIGNAL(registerUserReq(qint32, QString)),
            this, SLOT(registerNewUser(qint32, QString)) );
    connect(m_server, SIGNAL(subscribeStock(qint32, qint32)),
            this, SLOT(subscribeStock(qint32, qint32)) );
    connect(m_server, SIGNAL(unsubscribeStock(qint32, qint32)),
            this, SLOT(unsubscribeStock(qint32, qint32)) );
    connect(m_server, SIGNAL(sellStock(qint32, Offer)),
            this, SLOT(sellStock(qint32, Offer)) );
    connect(m_server, SIGNAL(buyStock(qint32, Offer)),
            this, SLOT(buyStock(qint32, Offer)) );
    connect(m_server, SIGNAL(getStocks(qint32)),
            this, SLOT(getStocks(qint32)) );

    qDebug() << "[Market] Serwer jest aktywny.";
}

Market::~Market()
{
    qDebug() << "[Marker] Zamykanie połączenia z bazą danych...";
    m_database.close();
    qDebug() << "[Market] Połączenie z bazą danych jest zamknięte.\n"
             << "[Market] Zamykanie serwera.";
    delete m_server;
    qDebug() << "[Market] Serwer został zamknięty.";
}

void Market::registerNewUser(qint32 tmpUserId, QString password)
{
    //zapytanie rejestrujące użytkownika powinno zwracać dokłądnie 1 rekord z nowym id
    //QString queryString;

    QSqlQuery query(m_database);
    //
    query.prepare("SELECT nowy_uzytkownik(:password);");
    query.bindValue(":password", password);
    //
    query.setForwardOnly(true);

    m_database.transaction();

    query.exec();

    m_database.commit();
    /*
     *  TODO:
     *      Implementacja odpowiednich zachowania.
     */
    if(query.first())
    {
        RegisterUserRespMsg Msg(query.value(0).toInt());
        m_server->send(Msg, tmpUserId);
    }
    else
    {
        qDebug() << "[Market] Nowy indentyfikator użytkownika nie został"
                 << "zwrócony. Błąd: " << query.lastError().text();
    }

}

void Market::subscribeStock(qint32 userId, qint32 stockId)
{
    //zapytanie nie powinno nic zwracac

    QString queryString;

    QSqlQuery query(m_database);
    query.setForwardOnly(true);

    m_database.transaction();

    query.exec(queryString);


    +m_database.commit();
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
        Transaction tmpTran(query.value(0).toInt(), query.value(1).toInt(),
                            query.value(2).toInt(), query.value(3).toInt(),
                            query.value(4).toInt(), query.value(5).toDouble());


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
                Offer(query.value(1).toInt(), query.value(2).toInt(),
                      query.value(3).toInt(), query.value(4).toInt(),
                      query.value(5).toDouble()),
                Offer(query.value(6).toInt(), query.value(7).toInt(),
                      query.value(8).toInt(), query.value(9).toInt(),
                      query.value(10).toDouble()));

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
