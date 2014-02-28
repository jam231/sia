#ifndef SERWER_H
#define SERWER_H


#include "connection.h"
#include "utilities.h"

#include <Responses/registerusersuccessmsg.h>
#include <Responses/okmsg.h>

#include <exception>

#include <QObject>
#include <QTcpServer>
#include <QHash>

class TcpConnectionError : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Tcp connection error.";
    }
};


using std::map;

class Server : public QObject
{    
    Q_OBJECT

    QHash<NetworkProtocol::DTO::Types::UserIdType, Connection*> _userConnections;
    QTcpServer* _server;

public:
    explicit Server(QObject *parent = 0, int portNumber = 6666);
    ~Server();
public slots:
    void send(NetworkProtocol::Responses::Response& msg, Connection*);
    void send(NetworkProtocol::Responses::RegisterUserSuccess& msg, Connection*);
    void send(NetworkProtocol::Responses::Ok& msg, Connection*connection, NetworkProtocol::DTO::Types::UserIdType userId);
    void send(NetworkProtocol::Responses::Response& msg);
    void send(NetworkProtocol::Responses::Response& msg, NetworkProtocol::DTO::Types::UserIdType userId);

signals:

    void registerUserRequestFromServer(Connection*, QString password);
    void loginUserRequestFromServer(Connection *connection, NetworkProtocol::DTO::Types::UserIdType userId,
                                    QString password);

    void sellStock(NetworkProtocol::DTO::Types::UserIdType userId, NetworkProtocol::DTO::Types::StockIdType stockId,
                   NetworkProtocol::DTO::Types::AmountType amount, NetworkProtocol::DTO::Types::PriceType price);
    void buyStock(NetworkProtocol::DTO::Types::UserIdType userId, NetworkProtocol::DTO::Types::StockIdType stockId,
                  NetworkProtocol::DTO::Types::AmountType amount, NetworkProtocol::DTO::Types::PriceType price);

    void getMyStocks(NetworkProtocol::DTO::Types::UserIdType userId);
    void getMyOrders(NetworkProtocol::DTO::Types::UserIdType userId);
    void getStockInfo(NetworkProtocol::DTO::Types::UserIdType userId,
                      NetworkProtocol::DTO::Types::StockIdType stockId);

    void cancelOrder(NetworkProtocol::DTO::Types::UserIdType userId,
                     NetworkProtocol::DTO::Types::StockIdType orderId);

private slots:
    void addNewConnection();
    void disconnectUser(NetworkProtocol::DTO::Types::UserIdType userId);


};

#endif // SERWER_H
