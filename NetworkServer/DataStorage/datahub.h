#ifndef DATAHUB_H
#define DATAHUB_H

#include <QThread>
#include <QSqlDriver>
#include <QSqlDatabase>
#include <QEventLoop>

#include <logger.h>

#include <../NetworkProtocol/DataTransferObjects/types.h>

Q_DECLARE_METATYPE(NetworkProtocol::DTO::Types::UserIdType)
Q_DECLARE_METATYPE(NetworkProtocol::DTO::Types::OrderIdType)
Q_DECLARE_METATYPE(NetworkProtocol::DTO::Types::AmountType)
Q_DECLARE_METATYPE(NetworkProtocol::DTO::Types::PriceType)

/*
 *  Class for handling notifications.
 */
class AbstractDataHub : public QThread
{
    Q_OBJECT
public:
    virtual ~AbstractDataHub() {}

signals:
    void orderChange(NetworkProtocol::DTO::Types::UserIdType orderOwner,
                     NetworkProtocol::DTO::Types::OrderIdType,
                     NetworkProtocol::DTO::Types::AmountType,
                     NetworkProtocol::DTO::Types::PriceType);

    void orderCompleted(NetworkProtocol::DTO::Types::UserIdType orderOwner,
                        NetworkProtocol::DTO::Types::OrderIdType);

    void bestOrderChange(NetworkProtocol::DTO::Types::StockIdType,
                         NetworkProtocol::DTO::Types::Order::OrderType,
                         NetworkProtocol::DTO::Types::AmountType,
                         NetworkProtocol::DTO::Types::PriceType);

    void lastTransactionChange(NetworkProtocol::DTO::Types::StockIdType,
                               NetworkProtocol::DTO::Types::AmountType,
                               NetworkProtocol::DTO::Types::PriceType,
                               QString dateTime);

};



class PostgreDataHub : public AbstractDataHub
{
    Q_OBJECT

    QHash<QString, QString> _config;
    std::shared_ptr<AbstractLoggerFactory> _loggerFactory;

    QSqlDatabase _handle;

    static const QString ORDER_COMPLETION_CHANNEL;
    static const QString ORDER_CHANGE_CHANNEL;
    static const QString BEST_ORDER_CHANGE_CHANNEL;
    static const QString LAST_TRANSACTION_CHANGE_CHANNEL;

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
    void handleLastTransactionChange(const QVariant& payload);

protected slots:
    void notificationHandler(const QString& channelName,
                             QSqlDriver::NotificationSource source,
                             const QVariant& payload);
};

#endif // DATAHUB_H
