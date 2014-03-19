#ifndef DATAHUB_H
#define DATAHUB_H

#include <QObject>
#include <QRunnable>
#include <QSqlDriver>
#include <QEventLoop>

#include "datastorage.h"

#include <logger.h>

#include <../NetworkProtocol/DataTransferObjects/types.h>

/*
 *  Class for handling notifications from outside.
 */
class AbstractDataHub : public QObject, public QRunnable
{
    Q_OBJECT
public:
    virtual ~AbstractDataHub() {}
signals:
    void orderChange(NetworkProtocol::DTO::Types::UserIdType order_owner,
                     NetworkProtocol::DTO::Types::OrderIdType,
                     NetworkProtocol::DTO::Types::AmountType,
                     NetworkProtocol::DTO::Types::PriceType);

    void orderCompleted(NetworkProtocol::DTO::Types::UserIdType order_owner,
                        NetworkProtocol::DTO::Types::OrderIdType);

    void bestOrderChange(NetworkProtocol::DTO::Types::StockIdType,
                         NetworkProtocol::DTO::Types::AmountType,
                         NetworkProtocol::DTO::Types::PriceType);
};



class PostgreDataHub : public AbstractDataHub
{
    Q_OBJECT


    QEventLoop _event_loop;
    QHash<QString, QString> _config;
    std::shared_ptr<AbstractLoggerFactory> _loggerFactory;

    QSqlDatabase _handle;

    static const QString ORDER_COMPLETION_CHANNEL;
    static const QString ORDER_CHANGE_CHANNEL;
    static const QString BEST_ORDER_CHANGE_CHANNEL;
public:

    PostgreDataHub(std::shared_ptr<AbstractLoggerFactory> loggerFactory,
                   const QHash<QString, QString>& config);

    void run();

    virtual ~PostgreDataHub();

protected:
    void establishConnection();

    void handleOrderCompleted(const QVariant& payload);
    void handleOrderChange(const QVariant& payload);
    void handleBestOrderChange(const QVariant& payload);

protected slots:
    void notificationHandler(const QString& channel_name,
                              QSqlDriver::NotificationSource source,
                              const QVariant& payload);
};

#endif // DATAHUB_H
