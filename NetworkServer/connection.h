#ifndef CONNECTION_H
#define CONNECTION_H

#include <Responses/response.h>
#include <Responses/shownewordermsg.h>


#include "utilities.h"

#include <QObject>
#include <QByteArray>
#include <QTcpSocket>

class Connection : public QObject
{
    Q_OBJECT
    static const qint32 NOT_ASSIGNED = -1;

    QByteArray _buffer;

    QTcpSocket* _socket;
    NetworkProtocol::DTO::Types::UserIdType _userId;
    QSet<NetworkProtocol::DTO::Types::StockIdType> _subscribedStocks;

public:
    Connection(QTcpSocket* socket, QObject *parent = 0);
    ~Connection();
    NetworkProtocol::DTO::Types::UserIdType userId() const;
    bool isUserAssigned() const;
    void setUserId(NetworkProtocol::DTO::Types::UserIdType);
    //dopiero od tego momentu zaczynamy odbierać wiadomości
    void start();
    void addSubscription(NetworkProtocol::DTO::Types::StockIdType stockId);
    void dropSubscription(NetworkProtocol::DTO::Types::StockIdType stockId);
public slots:
    bool send(NetworkProtocol::Responses::Response& msg);
    bool send(NetworkProtocol::Responses::ShowNewOrder msg);
private slots:
    void disconect();
    void processIncomingMessages();
    bool processMessage();

signals:
    void disconnected(NetworkProtocol::DTO::Types::UserIdType userId);

    void loginUserRequestFromConnection(Connection* connection, NetworkProtocol::DTO::Types::UserIdType, QString);
    void registerUserRequestFromConnection(Connection* connection, QString password);

    void sellStock(NetworkProtocol::DTO::Types::UserIdType userId, NetworkProtocol::DTO::Types::StockIdType stockId,
                   NetworkProtocol::DTO::Types::AmountType amount, NetworkProtocol::DTO::Types::PriceType price);
    void buyStock(NetworkProtocol::DTO::Types::UserIdType userId, NetworkProtocol::DTO::Types::StockIdType stockId,
                   NetworkProtocol::DTO::Types::AmountType amount, NetworkProtocol::DTO::Types::PriceType price);

    void getMyStocks(NetworkProtocol::DTO::Types::UserIdType userId);
    void getMyOrders(NetworkProtocol::DTO::Types::UserIdType userId);
    void getStockInfo(NetworkProtocol::DTO::Types::UserIdType userId, NetworkProtocol::DTO::Types::StockIdType stockId);

    void cancelOrder(NetworkProtocol::DTO::Types::UserIdType userId, NetworkProtocol::DTO::Types::OrderIdType orderId);
};

#endif // CONNECTION_H
