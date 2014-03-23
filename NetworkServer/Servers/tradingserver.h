#ifndef TRADINGSERVER_H
#define TRADINGSERVER_H

#include <memory>

#include <QObject>
#include <QRunnable>
#include <QTcpSocket>
#include <QEventLoop>

#include <../NetworkProtocol/DataTransferObjects/types.h>

#include <../NetworkProtocol/Requests/request.h>

#include <../Utilities/logger.h>

#include <DataStorage/datastorage.h>

#include "connections.h"

#include <Utils/utils.h>


/*
 *  Problems:
 *
 *
 * When adding userConnection, how to handle deleting QTcpSocket* in sender of
 * socket_descriptor ?
 *
 * Who is the sender of descriptor ?
 *
 * QHash needs to be thread safe.
 *
 *
 *
 */
class TradingServer : public QThread
{
    Q_OBJECT
    QHash<NetworkProtocol::DTO::Types::UserIdType,
          std::shared_ptr<UserConnection> > _userConnections;
    std::shared_ptr<SharedSet<NetworkProtocol::DTO::Types::UserIdType> > _online_users;

    std::shared_ptr<AbstractLoggerFactory> _loggerFactory;
    std::shared_ptr<AbstractDataStorageFactory> _dataStorageFactory;

public:
    TradingServer(std::shared_ptr<AbstractLoggerFactory> loggerFactory,
                  std::shared_ptr<AbstractDataStorageFactory> datastorageFactory,
                  std::shared_ptr<SharedSet<NetworkProtocol::DTO::Types::UserIdType> > );
protected:
    virtual void run();
public:
    void handleRequest(std::shared_ptr<AbstractLogger>, NetworkProtocol::Requests::Request*,
                       NetworkProtocol::DTO::Types::UserIdType);
public slots:
   void addUserConnection(UserConnection*);
   void processMessageFrom(NetworkProtocol::DTO::Types::UserIdType);
   void removeConnection(NetworkProtocol::DTO::Types::UserIdType);
};



#endif // TRADINGSERVER_H
