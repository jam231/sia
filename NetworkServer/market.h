#ifndef MARKET_H
#define MARKET_H


#include "server.h"
#include "configmanager.h"

#include <exception>

#include <Responses/showbestordermsg.h>

#include "Utils/utils.h"

#include <DataTransferObjects/order.h>
#include <DataTransferObjects/lasttransaction.h>
#include <DataTransferObjects/types.h>

#include <QTimer>
#include <QObject>
#include <QString>
#include <QSqlDatabase>
#include <QSqlDriver>
#include <QPair>


class DatabaseError : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Database error";
    }
};

class Market : public QObject
{
    Q_OBJECT

    static const QString BUY_TRANSACTIONS_CHANNEL;// = "ch_zlecenia_kupna";
    static const QString SELL_TRANSACTIONS_CHANNEL;// = "ch_zlecenia_sprzedazy";
    static const QString CHANGE_CHANNEL;// = "ch_zmiana";

    Server* _server;
    QSqlDatabase _database;

    QTimer* _sessionOnTimer;
    QTimer* _sessionOffTimer;

    QHash<NetworkProtocol::DTO::Types::StockIdType,
          std::shared_ptr<NetworkProtocol::DTO::LastTransaction> > _cachedLastTransaction;
    QHash<NetworkProtocol::DTO::Types::StockIdType,
          std::shared_ptr<NetworkProtocol::DTO::BestOrder> > _cachedBestSellOrders;
    QHash<NetworkProtocol::DTO::Types::StockIdType,
          std::shared_ptr<NetworkProtocol::DTO::BestOrder> > _cachedBestBuyOrders;

protected:

   void changeCachedBestSellOrders(NetworkProtocol::DTO::Types::StockIdType stockId);
   void changeCachedBestBuyOrders(NetworkProtocol::DTO::Types::StockIdType stockId);

public:
    Market(const ConfigManager<>& config, QObject* parent = 0);
    ~Market();

public slots:
    void registerNewUser(Connection*, QString password);
    void loginUser(Connection*, NetworkProtocol::DTO::Types::UserIdType userId, QString password);

    void startSession();
    void stopSession();

    void notificationHandler(const QString&, QSqlDriver::NotificationSource, const QVariant &);

    void sellStock(NetworkProtocol::DTO::Types::UserIdType userId, NetworkProtocol::DTO::Types::StockIdType stockId,
                   NetworkProtocol::DTO::Types::AmountType amount, NetworkProtocol::DTO::Types::PriceType price);
    void buyStock(NetworkProtocol::DTO::Types::UserIdType userId, NetworkProtocol::DTO::Types::StockIdType stockId,
                  NetworkProtocol::DTO::Types::AmountType amount, NetworkProtocol::DTO::Types::PriceType price);

    void getMyStocks(NetworkProtocol::DTO::Types::UserIdType userId);
    void getMyOrders(NetworkProtocol::DTO::Types::UserIdType userId);
    void getStockInfo(NetworkProtocol::DTO::Types::UserIdType userId, NetworkProtocol::DTO::Types::StockIdType stockId);

    void cancelOrder(NetworkProtocol::DTO::Types::UserIdType userId, NetworkProtocol::DTO::Types::StockIdType stockId);
};

#endif // MARKET_H
