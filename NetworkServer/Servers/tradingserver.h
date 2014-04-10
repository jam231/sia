#ifndef TRADINGSERVER_H
#define TRADINGSERVER_H

#include <memory>

#include <QObject>
#include <QRunnable>
#include <QTcpSocket>
#include <QEventLoop>

#include <../NetworkProtocol/DataTransferObjects/types.h>

#include <../NetworkProtocol/Requests/request.h>
#include <../NetworkProtocol/Requests/buystockmsg.h>
#include <../NetworkProtocol/Requests/sellstockmsg.h>
#include <../NetworkProtocol/Requests/cancelordermsg.h>
#include <../NetworkProtocol/Requests/getmyordersmsg.h>
#include <../NetworkProtocol/Requests/getmystocksmsg.h>
#include <../NetworkProtocol/Requests/subscribestockmsg.h>
#include <../NetworkProtocol/Requests/unsubscribestockmsg.h>

#include <../Utilities/logger.h>

#include <DataStorage/datastorage.h>
#include <DataStorage/datahub.h>

#include "connections.h"

#include <Utils/utils.h>


class TradingServer : public QThread
{
    Q_OBJECT
    QHash<NetworkProtocol::DTO::Types::UserIdType,
          UserConnection* >                                     _userConnections;

    std::shared_ptr<SharedSet<NetworkProtocol::DTO::Types::UserIdType> > _online_users;


    std::shared_ptr<AbstractLoggerFactory>                      _loggerFactory;
    std::shared_ptr<AbstractDataStorageFactory>                 _dataStorageFactory;

    QHash<NetworkProtocol::DTO::Types::StockIdType,
          QSet<NetworkProtocol::DTO::Types::UserIdType> >       _stock_subscribers;

    std::shared_ptr<NetworkProtocol::DTO::LastTransaction>      _lastTransaction;

    QHash<NetworkProtocol::DTO::Types::StockIdType,
          std::shared_ptr<NetworkProtocol::DTO::BestOrder> >    _bestBuyOrder;

    QHash<NetworkProtocol::DTO::Types::StockIdType,
          std::shared_ptr<NetworkProtocol::DTO::BestOrder> >    _bestSellOrder;

public:
    TradingServer(std::shared_ptr<AbstractLoggerFactory> loggerFactory,
                  std::shared_ptr<AbstractDataStorageFactory> datastorageFactory,
                  std::shared_ptr<SharedSet<NetworkProtocol::DTO::Types::UserIdType> > );
protected:
    virtual void run();

    void loadSubscriptionSet();
public:

    void handleRequest(std::shared_ptr<AbstractLogger>,
                       NetworkProtocol::Requests::BuyStock*,
                       NetworkProtocol::DTO::Types::UserIdType);

    void handleRequest(std::shared_ptr<AbstractLogger>,
                       NetworkProtocol::Requests::SellStock*,
                       NetworkProtocol::DTO::Types::UserIdType);

    void handleRequest(std::shared_ptr<AbstractLogger>,
                       NetworkProtocol::Requests::CancelOrder*,
                       NetworkProtocol::DTO::Types::UserIdType);

    void handleRequest(std::shared_ptr<AbstractLogger>,
                       NetworkProtocol::Requests::GetMyOrders*,
                       NetworkProtocol::DTO::Types::UserIdType);

    void handleRequest(std::shared_ptr<AbstractLogger>,
                       NetworkProtocol::Requests::GetMyStocks*,
                       NetworkProtocol::DTO::Types::UserIdType);

    void handleRequest(std::shared_ptr<AbstractLogger>,
                       NetworkProtocol::Requests::SubscribeStock*,
                       NetworkProtocol::DTO::Types::UserIdType);

    void handleRequest(std::shared_ptr<AbstractLogger>,
                       NetworkProtocol::Requests::UnsubscribeStock*,
                       NetworkProtocol::DTO::Types::UserIdType);

    void handleRequest(std::shared_ptr<AbstractLogger>,
                       NetworkProtocol::Requests::Request*,
                       NetworkProtocol::DTO::Types::UserIdType);

public slots:
   void connectDataHub(const AbstractDataHub&);

   void addUserConnection(UserConnection*);

   void processMessageFrom(NetworkProtocol::DTO::Types::UserIdType);

   void removeConnection(NetworkProtocol::DTO::Types::UserIdType);

   void orderCompleted(NetworkProtocol::DTO::Types::UserIdType,
                       NetworkProtocol::DTO::Types::OrderIdType);

   void orderChange(NetworkProtocol::DTO::Types::UserIdType,
                    NetworkProtocol::DTO::Types::OrderIdType,
                    NetworkProtocol::DTO::Types::AmountType,
                    NetworkProtocol::DTO::Types::PriceType);

   void newLastTransaction(NetworkProtocol::DTO::LastTransaction*);
   void newBestBuyOrder(NetworkProtocol::DTO::BestOrder*);
   void newBestSellOrder(NetworkProtocol::DTO::BestOrder*);
};



#endif // TRADINGSERVER_H
