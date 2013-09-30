#include "market.h"

#include "transaction.h"
#include "stock.h"
#include "configmanager.h"


#include <Responses/registeruserrespok.h>
#include <Responses/loginuserrespfail.h>
#include <Responses/loginuserrespok.h>
#include <Responses/getmystocksrespmsg.h>
#include <Responses/ordermsg.h>
#include <Responses/getmyordersrespmsg.h>
#include <Responses/getstockinforespmsg.h>
#include <Responses/orderidresp.h>
#include <Responses/registeruserrespfail.h>

#include <Responses/buytransactionmsg.h>
#include <Responses/selltransactionmsg.h>
#include <Responses/transactionchange.h>

#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>



const QString Market::BUY_TRANSACTIONS_CHANNEL = "ch_zlecenia_kupna";
const QString Market::SELL_TRANSACTIONS_CHANNEL = "ch_zlecenia_sprzedazy";
const QString Market::CHANGE_CHANNEL = "ch_zmiana";

using namespace NetworkProtocol;
using namespace NetworkProtocol::Responses;
using namespace NetworkProtocol::DTO;

Market::Market(const ConfigManager<>& config, QObject* parent)
    : QObject(parent)
{
    qDebug() << "[Market] Ładowanie danych konfiguracyjnych...";

    qint32 sessionOnInterval = config.intAt("SESSION ON DURATION");
    qint32 sessionOffInterval = config.intAt("SESSION OFF DURATION");

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

    m_database.driver()->subscribeToNotification(BUY_TRANSACTIONS_CHANNEL);
    m_database.driver()->subscribeToNotification(SELL_TRANSACTIONS_CHANNEL);
    m_database.driver()->subscribeToNotification(CHANGE_CHANNEL);

    qDebug() << "[Market] Ustanowiono połączenie z bazą danych.";

    //qDebug() << "Database tables:\n" << m_database.tables();

    qDebug() << "[Market] Uruchamianie serwera...";
    m_server = new Server(this, serverPort);

    m_sessionOffTimer = new QTimer(this);
    m_sessionOffTimer->setInterval(sessionOffInterval * 1000);

    m_sessionOnTimer = new QTimer(this);
    m_sessionOnTimer->setInterval(sessionOnInterval * 1000);


    connect(m_server, SIGNAL(registerUserRequestFromServer(Connection*, QString)),
            this,     SLOT(registerNewUser(Connection*, QString)) );

    connect(m_server, SIGNAL(loginUserRequestFromServer(Connection*, qint32, QString)),
            this,     SLOT(loginUser(Connection*, qint32, QString)));

    connect(m_database.driver(), SIGNAL(notification(const QString&, QSqlDriver::NotificationSource,
                                                     const QVariant&)),
            this,               SLOT(notificationHandler(const QString&, QSqlDriver::NotificationSource,
                                                         const QVariant&)));

    connect(m_server, SIGNAL(sellStock(qint32, qint32, qint32, qint32)),
            this,     SLOT(sellStock(qint32, qint32, qint32, qint32)));

    connect(m_server, SIGNAL(buyStock(qint32, qint32, qint32, qint32)),
            this,     SLOT(buyStock(qint32, qint32, qint32, qint32)));

    connect(m_server, SIGNAL(getMyStocks(qint32)),
            this,     SLOT(getMyStocks(qint32)));

    connect(m_server, SIGNAL(getMyOrders(qint32)),
            this, SLOT(getMyOrders(qint32)));

    connect(m_server, SIGNAL(getStockInfo(qint32, qint32)),
            this,     SLOT(getStockInfo(qint32, qint32)));

    connect(m_server, SIGNAL(cancelOrder(qint32,qint32)),
            this,     SLOT(cancelOrder(qint32,qint32)));

    connect(m_sessionOnTimer, SIGNAL(timeout()),
            this,             SLOT(stopSession()));

    connect(m_sessionOffTimer, SIGNAL(timeout()),
            this,              SLOT(startSession()));

    qDebug() << "[Market] Serwer jest aktywny.";
    startSession();
}

Market::~Market()
{
    m_sessionOnTimer->stop();
    m_sessionOffTimer->stop();
    delete m_sessionOnTimer;
    delete m_sessionOffTimer;

    qDebug() << "[Marker] Zamykanie połączenia z bazą danych...";
    m_database.close();
    qDebug() << "[Market] Połączenie z bazą danych zostało zamknięte.";
    qDebug() << "[Market] Zamykanie serwera.";
    delete m_server;
    qDebug() << "[Market] Serwer został zamknięty.";
}


void Market::registerNewUser(Connection* connection, QString password)
{

    qDebug() << "[Market] Rejestrowane nowego uzytkownika w toku...";
    QSqlQuery query(m_database);
    //
    query.prepare("SELECT nowy_uzytkownik(:password);");
    query.bindValue(":password", password);
    //
    query.setForwardOnly(true);

    m_database.transaction();

    query.exec();

    m_database.commit();
    if(query.first())
    {
        RegisterUserRespOk response(static_cast<qint32>(query.value(0).toInt()));
        qDebug() << "[Market] Wysyłanie identyfikatora nowemu użytkownikowi.";
        m_server->send(response, connection);
    }
    else
    {
        qDebug() << "[Market] Nowy indentyfikator użytkownika nie został"
                 << "zwrócony. Błąd: " << query.lastError().text();
        RegisterUserRespFail response("Niepoprawne hasło.");
        m_server->send(response, connection);

    }

}

void Market::loginUser(Connection* connection, qint32 userId, QString password)
{
    qDebug() << "[Market] Wyszukiwanie użytkownika o id =" << userId
             << "w bazie...";
    QSqlQuery query(m_database);

    query.prepare("SELECT haslo FROM uzytkownik WHERE id_uz = :id ;");
    query.bindValue(":id", userId);

    query.setForwardOnly(true);

    m_database.transaction();

    query.exec();

    m_database.commit();

    QString failReason;
    if(query.first())
    {
        QString pswd = query.value(0).toString();
        if(password == pswd)
        {
            qDebug() << "[Market] Wysyłanie potwierdzenia udanej "\
                        "autoryzacji do uzytkownika id =" << userId;
            LoginUserRespOk responseMsg;
            m_server->send(responseMsg, connection, userId);
            return;
        }
        else
        {
            qDebug() << "[Market] Użytkownik id =" << userId
                     << "podał błędne hasło.";
            failReason = "Błędne hasło";
        }
    }
    else
    {
        qDebug() << "[Market] Nie znaleziono użytkownika w bazie.";
        failReason = "Błędne id";
    }
    LoginUserRespFail responseMsg(failReason);
    m_server->send(responseMsg, connection);
}

void Market::startSession()
{
    m_sessionOffTimer->stop();

    for (qint32 i=2;i<=21;i++) {
        changeCachedBestSellOrders(i);
        changeCachedBestBuyOrders(i);
    }


    QSqlQuery query(m_database);
    query.prepare("SELECT rozpocznij_sesje();");
    GetMyStocksRespMsg msg;
    query.setForwardOnly(true);

    m_database.transaction();

    if (query.exec() == false)
        qDebug() << "[Market] Błąd przy rozpoczynaniu sesji " << (query.lastError().text());



    m_database.commit();

    m_sessionOnTimer->start();

    qDebug() << "[Market] Sesja rozpoczęta.";
}

void Market::stopSession()
{
    m_sessionOnTimer->stop();
    QSqlQuery query(m_database);
    query.prepare("SELECT zakoncz_sesje();");

    query.setForwardOnly(true);

    m_database.transaction();

    query.exec();

    m_database.commit();

    m_sessionOffTimer->start();

    qDebug() << "[Market] Sesja zamknięta.";
}

void Market::notificationHandler(const QString& channelName,
                                 QSqlDriver::NotificationSource source,
                                 const QVariant& payload)
{

    if(BUY_TRANSACTIONS_CHANNEL == channelName)
    {
        QStringList data = payload.toString().split('|');
        if(data.size() == 3)
        {
            // TODO: Sprawdzanie poprawnosci !
            qint32 orderId = data[0].toInt(),
                   amount = data[1].toInt(),
                   userId = data[2].toInt();

            //changeCachedBestBuyOrders(userId);

            BuyTransactionMsg msg(orderId, amount);
            m_server->send(msg, userId);
        }
        else
            qDebug() << "[Market] " << BUY_TRANSACTIONS_CHANNEL
                     << "powinnien zawierac 3 argumenty, a zawiera"
                     << data.size();
    }
    else if(SELL_TRANSACTIONS_CHANNEL == channelName)
    {
        QStringList data = payload.toString().split('|');
        // TODO: Sprawdzanie poprawnosci !
        if(data.size() == 3)
        {
            qint32 orderId = data[0].toInt(),
                   amount = data[1].toInt(),
                   userId = data[2].toInt();

            //changeCachedBestSellOrders(userId);

            SellTransactionMsg msg(orderId, amount);
            m_server->send(msg, userId);
        }
        else
            qDebug() << "[Market] " << SELL_TRANSACTIONS_CHANNEL
                     << "powinnien zawierac 3 argumenty, a zawiera"
                     << data.size();
    }
    else if(CHANGE_CHANNEL == channelName)
    {
        QStringList data = payload.toString().split('|');
        if(data.size() == 4)
        {
            qint32 stockId = data[0].toInt(),
                   amount = data[1].toInt(),
                   price = data[2].toInt();
            QString date = data[3];

            TransactionChange msg(stockId, amount, price, date);
            m_server->send(msg);

            m_cachedLastTransaction[stockId] = LastTransaction(date, amount, price);

            changeCachedBestBuyOrders(stockId);
            // TODO: Jak już gdzieś wspomniałem być może
            //       dałoby się to zrobić optymalniej.
            if(m_cachedBestBuyOrders.contains(stockId))
            {
                BestOrderMsg msg(Types::OrderType::BUY, stockId, m_cachedBestBuyOrders[stockId].first,
                                 m_cachedBestBuyOrders[stockId].second);
                m_server->send(msg);
            }
            else
            {
                BestOrderMsg msg(Types::OrderType::BUY, stockId, 0, 0);
                m_server->send(msg);
            }

            changeCachedBestSellOrders(stockId);
            // TODO: Jak już gdzieś wspomniałem być może
            //       dałoby się to zrobić optymalniej.
            if(m_cachedBestSellOrders.contains(stockId))
            {
                BestOrderMsg msg(Types::OrderType::SELL, stockId,m_cachedBestSellOrders[stockId].first,
                                 m_cachedBestSellOrders[stockId].second);
                //BestOrderMsg msg(m_cachedBestSellOrders[stockId]);
                m_server->send(msg);
            }
            else
            {
                BestOrderMsg msg(Types::OrderType::SELL, stockId, 0, 0);
                m_server->send(msg);
            }
        }
        else
            qDebug() << "[Market] " << CHANGE_CHANNEL
                     << "powinnien zawierac 4 argumenty, a zawiera"
                     << data.size();
    }
    else
    {
        qDebug() << "[Market] Nieobsłużony kanał " << channelName
                 << source << payload;
    }
}

void Market::sellStock(qint32 userId, qint32 stockId, qint32 amount, qint32 price)
{
    qDebug() << "[Market] Użytkownik o id =" << userId
             << "zleca transakcje sprzedaży" << amount
             << "dobra o id =" << stockId
             << "za cenę" << price;

    QSqlQuery query(m_database);
    //
    query.prepare("SELECT * FROM zlec_sprzedaz(:userId, :stockId, :amount, :price);");
    /* TODO:
     *  Jeżeli się da to naprawić.
     */
    query.bindValue(":userId", userId);
    query.bindValue(":stockId", stockId);
    query.bindValue(":amount", amount);
    query.bindValue(":price", price);
    //
    query.setForwardOnly(true);

    m_database.transaction();

    query.exec();

    m_database.commit();

    if(!query.lastError().isValid())
    {
        if(query.first())
        {
            OrderIdResp msg(query.value(0).toInt());
            m_server->send(msg, userId);
        }
        else
        {
            qDebug() << "[Market] Błąd nadania id dla zlecenia sprzedaży";
            OrderIdResp msg(-1);
            m_server->send(msg, userId);
        }

        auto lastBestOrder = m_cachedBestSellOrders[stockId];
        changeCachedBestSellOrders(stockId);

        //Order order(Order::SELL, stockId, amount, price);
        // Wyślij wszystkim subskrybentom.
        //OrderMsg msg(order);
        //m_server->send(msg);

        // if changed then send!
        if(m_cachedBestSellOrders[stockId] != lastBestOrder)
        {
            BestOrderMsg msg(Types::OrderType::SELL, stockId,m_cachedBestSellOrders[stockId].first,
                             m_cachedBestSellOrders[stockId].second);
            m_server->send(msg);
        }
    }
    else
    {
        qDebug() << "[Market] Błąd przy zleceniu sprzedaży"
                 << query.lastError().text();
        OrderIdResp msg(-1);
        m_server->send(msg, userId);
    }
}

void Market::buyStock(qint32 userId, qint32 stockId, qint32 amount, qint32 price)
{
    qDebug() << "[Market] Użytkownik o id =" << userId
              << "zleca transakcje kupna" << amount
              << "dobra o id =" << stockId
              << "za cenę" << price;

     QSqlQuery query(m_database);

     query.prepare("SELECT * FROM zlec_kupno(:userId, :stockId, :amount, :price);");
     /* TODO:
      *  Jeżeli się da to naprawić.
      */
     query.bindValue(":userId", userId);
     query.bindValue(":stockId", stockId);
     query.bindValue(":amount", amount);
     query.bindValue(":price", price);


     query.setForwardOnly(true);

     m_database.transaction();

     query.exec();

     m_database.commit();
     if(!query.lastError().isValid())
     {
         if(query.first())
         {
             OrderIdResp msg(query.value(0).toInt());
             m_server->send(msg, userId);
         }
         else
         {
             qDebug() << "[Market] Błąd nadania id dla zlecenia kupna";
             OrderIdResp msg(-1);
             m_server->send(msg, userId);
         }

         auto lastBestOrder = m_cachedBestBuyOrders[stockId];
         changeCachedBestBuyOrders(stockId);

         // Order order(Order::BUY, stockId, amount, price);
         // Wyślij wszytkim subskrybentom.
         //OrderMsg msg(order);
         //m_server->send(msg);

         // if changed then send!]
         if(m_cachedBestBuyOrders[stockId] != lastBestOrder)
         {
             // FIX
             // Meh, wczesniej byla wiadomosc bestOrder, ktora sie po prostu wysylalo, a teraz to jakis chuj
             // za kazdym razem trzeba sie jebac z czyms takim:
             BestOrderMsg msg(Types::OrderType::BUY, stockId, m_cachedBestBuyOrders[stockId].first,
                              m_cachedBestBuyOrders[stockId].second);
             m_server->send(msg);
         }
     }
     else
     {
         qDebug() << "[Market] Błąd przy zleceniu kupna"
                  << query.lastError().text();
         OrderIdResp msg(-1);
         m_server->send(msg, userId);
     }
}


void Market::changeCachedBestSellOrders(qint32 stockId)
{
    QSqlQuery query(m_database);

    query.prepare("SELECT * FROM najlepsza_sprzedaz(:stockId);");
    query.bindValue(":stockId", stockId);

    query.setForwardOnly(true);

    m_database.transaction();

    query.exec();

    m_database.commit();
    if(query.first())
    {
        if(query.value(0).isValid() && query.value(1).isValid())
        {
            //qDebug() << "[Market] changeCachedSellBuyOrders: "
            //          << query.value(0).toInt() << " " << query.value(1).toInt();
            m_cachedBestSellOrders.insert(stockId,
                                          qMakePair(query.value(0).toInt(),
                                                    query.value(1).toInt()));
        }
        else
            qDebug() << "[Market] W changeCachedBestBuyOrders"
                     << "zwrócony rekord nie ma dwóch pol.";
    }
    // To znaczy, ze dla danej transakcji nie ma w ogole oferty...
    else
    {
        m_cachedBestSellOrders.remove(stockId);
    }
}

void Market::changeCachedBestBuyOrders(qint32 stockId)
{
    QSqlQuery query(m_database);

    query.prepare("SELECT * FROM najlepsze_kupno(:stockId);");
    query.bindValue(":stockId", stockId);

    query.setForwardOnly(true);

    m_database.transaction();

    query.exec();

    m_database.commit();
    if(query.first())
    {
        if(query.value(0).isValid() && query.value(1).isValid())
        {
           //qDebug() << "[Market] changeCachedBestBuyOrders: "
           //          << query.value(0).toInt() << " " << query.value(1).toInt();
            m_cachedBestBuyOrders.insert(stockId,
                                      qMakePair(query.value(0).toInt(),
                                                query.value(1).toInt()));
        }
        else
            qDebug() << "[Market] W changeCachedBestBuyOrders"
                     << "zwrócony rekord nie ma dwóch pol.";
    }
    // To znaczy, ze dla danej transakcji nie ma w ogole oferty...
    else
    {
        m_cachedBestBuyOrders.remove(stockId);
    }
}

void Market::getMyStocks(qint32 userId)
{
    qDebug() << "[Market] Użytkownik o id =" << userId
              << "prosi o listę zasobów";

     QSqlQuery query(m_database);

     query.prepare("SELECT id_zasobu, ilosc FROM dobra_uz(:userId);");

     query.bindValue(":userId", userId);


     query.setForwardOnly(true);

     m_database.transaction();

     query.exec();

     m_database.commit();

     GetMyStocksRespMsg msg;
     while (query.next())
         if(query.value(0).isValid() && query.value(1).isValid())
                msg.addStock(query.value(0).toInt(), query.value(1).toInt());
         else
             qDebug() << "[Market] W getMyStocks"
                      << "zwrócony rekord nie ma dwóch pol.";

     m_server->send(msg, userId);
}

void Market::getMyOrders(qint32 userId)
{
    qDebug() << "[Market] Użytkownik o id =" << userId
              << "prosi o listę zlecen.";

     QSqlQuery query(m_database);

     query.prepare("SELECT typ, id_zlecenia, id_zasobu, ilosc, limit1 FROM zlecenia_uz(:userId);");

     query.bindValue(":userId", userId);


     query.setForwardOnly(true);

     m_database.transaction();

     query.exec();

     m_database.commit();

     GetMyOrdersRespMsg msg;
     while (query.next())
     {
         if(query.value(0).isValid() && query.value(1).isValid() &&
            query.value(2).isValid() && query.value(3).isValid())
         {
             // RETURNS TABLE(typ integer, id_zlecenia integer, id_zasobu integer,ilosc integer, limit1 integer)

             msg.addOrder(query.value(1).toInt(),
                          Types::toOrderType(query.value(0).toInt()), query.value(2).toInt(),
                          query.value(3).toInt(), query.value(4).toInt());
         }

        else
            qDebug() << "[Market] W getMyOrders"
                     << "zwrócony rekord nie ma czterech pol.";
     }
     m_server->send(msg, userId);
}

void Market::getStockInfo(qint32 userId, qint32 stockId)
{
    qDebug() << "[Market] Użytkownik o id =" << userId
              << "prosi o szczegoly zasobu o id=" << stockId;


    QPair<qint32, qint32> bestBuyOrder;
    QPair<qint32, qint32> bestSellOrder;
    LastTransaction lastTransaction;



    if(m_cachedBestBuyOrders.contains(stockId))
        bestBuyOrder = m_cachedBestBuyOrders[stockId];
    if(m_cachedBestSellOrders.contains(stockId))
        bestSellOrder = m_cachedBestSellOrders[stockId];
    if(m_cachedLastTransaction.contains(stockId))
        lastTransaction = m_cachedLastTransaction[stockId];

    GetStockInfoRespMsg msg(stockId, bestBuyOrder, bestSellOrder, lastTransaction);
    m_server->send(msg, userId);
}

/*
 *  To jest bardzo naiwne i niezbyt ladne, dodatkowo tu wychodzi brzydota
 *  zwiazana z podzialem zlecen na oddzielne tabele i fakt, ze id_zlecenia
 *  powinno byc jednoznacznie wyznaczone dla kazdego ze zlecen, niezaleznie od typu
 *                                              -jam231
 */
void Market::cancelOrder(qint32 userId, qint32 orderId)
{
    qDebug() << "[Market] Użytkownik " << userId << " prosi o anulowanie zlecenia " << orderId;
    QSqlQuery query1(m_database),
              query2(m_database);

    query1.prepare("DELETE FROM zlecenie_kupna AS zk WHERE zk.id_zlecenia = :orderId AND zk.id_uz = :userId;");
    query1.bindValue(":orderId", orderId);
    query1.bindValue(":userId", userId);

    query2.prepare("DELETE FROM zlecenie_sprzedazy AS zs WHERE zs.id_zlecenia = :orderId AND zs.id_uz = :userId;");
    query2.bindValue(":orderId", orderId);
    query2.bindValue(":userId", userId);


    query1.setForwardOnly(true);
    query2.setForwardOnly(true);

    m_database.transaction();

    query1.exec();
    query2.exec();

    m_database.commit();

    if(query1.lastError().isValid())
    {
        qDebug() << "[Market] Błąd przy anulowaniu zlecenia kupna"
                 << query1.lastError().text();
    }
    if(query2.lastError().isValid())
    {
        qDebug() << "[Market] Błąd przy anulowaniu zlecenia sprzedaży"
                 << query2.lastError().text();
    }
}
