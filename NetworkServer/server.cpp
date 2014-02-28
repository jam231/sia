#include "server.h"

#include <Responses/failuremsg.h>

using namespace NetworkProtocol;
using namespace NetworkProtocol::DTO;
using namespace NetworkProtocol::Responses;

Server::Server(QObject *parent, int portNumber)
    : QObject(parent), _server(new QTcpServer(this))
{
    qDebug() << "\t[Server] Tworzenie połączenia na porcie"
             << portNumber << "...";

    _server = new QTcpServer(this);

    connect(_server, SIGNAL(newConnection()),
            this,     SLOT(addNewConnection()));

    if(!_server->listen(QHostAddress::Any, portNumber))
    {
        qDebug() << "\t[Server] Wykryto błąd " << _server->errorString()
                 << "podczas tworzenia połączenia tcp.";
        throw TcpConnectionError();
    }
    qDebug() << "\t[Server] Ustanowiono połączenie tcp na porcie"
             << portNumber << ".";
}

Server::~Server()
{
    _server->close();
    qDebug() << "\t[Server] Połączenie tcp zostało zamknięte.";
    delete _server;
}

/* TODO:
 *      Ta metoda jest dość nie fajna, bo własciwie potrzebna
 *      tylko do wysyłania RegisterUserFail oraz LoginUserFail.
 *
 *      Być może warto by było przeładować send dla tych dwóch metod
 *      i zostawić tylko send(Response& msg, qint32 userId)
 */
void Server::send(Response& msg, Connection* connection)
{
    qDebug() << "\t[Server] Wysylanie wiadomsci: " << msg.type()
             << " do uzytkownika bez id";
    connection->send(msg);
}
/*
 *  To wymaga jeszcze trochę pracy.
 *  Wygląda to teraz tak, że w metodzie send robimy coś co moglibyśmy
 *  zrobić dużo wcześniej.
 *  Argumentem za takim zachowaniem jest seperacja warstwy komunikacji
 *  i warstwy związanej z bazą danych.
 *                                          --jam231
 */
void Server::send(Responses::RegisterUserSuccess& msg, Connection* connection)
{

    qDebug() << "\t[Server] Wysylanie wiadomsci: RegisterUserRespOk"
             << " z nowym id=" << msg.getUserId();

    connection->send(msg);
}

void Server::send(Responses::Ok& msg, Connection* connection,
                  DTO::Types::UserIdType userId)
{
    qDebug() << "\t[Server] Wysylanie wiadomsci: LoginUserRespOk"
             << " do uzytkownika z id=" << userId;
    /*
     *  Sprawdź, czy jest już taki użytkownik w systemie.
     *  Jeżeli jest to wyślij wiadomość świadczącą o niepowodzeniu.
     */
    if(!_userConnections.contains(userId))
    {
        _userConnections[userId] = connection;
        connection->setUserId(userId);

        connection->send(msg);
    }
    else
    {
        qDebug() << "\t[Server] Próba zalogowania na aktywne konto.";
        Responses::Failure response(Types::Failure::ALREADY_LOGGED);
        connection->send(response);
    }
}

void Server::send(Response& msg)
{
    qDebug() << "\t[Server] Wysylanie wiadomsci: " << msg.type()
             << " do wszystkich.";

    for(auto it = _userConnections.begin(); it != _userConnections.end(); it++)
    {
        it.value()->send(msg);
    }
}

void Server::send(Response& msg, Types::UserIdType userId)
{
    /* TODO:
     *
     *      Wydajniej by bylo za jednym zamachem zwrocic wartosc value
     *      o ile _userConnections zawiera klucz key
     */

    qDebug() << "\t[Server] Wysylanie wiadomsci: " << msg.type()
             << " do uzytkownika z id=" << userId;

    if(_userConnections.contains(userId))
        _userConnections[userId]->send(msg);
}

void Server::addNewConnection()
{
    qDebug() << "[Server] Nawiązywanie nowego połączenia.";
    Connection* newConnection = new Connection(_server->nextPendingConnection(),
                                         this);

    connect(newConnection, SIGNAL(disconnected(Types::UserIdType)),
              this,        SLOT(disconnectUser(Types::UserIdType)));

    connect(newConnection, SIGNAL(registerUserRequestFromConnection(Connection*, QString)),
              this,        SIGNAL(registerUserRequestFromServer(Connection *, QString)));

    connect(newConnection, SIGNAL(loginUserRequestFromConnection(Connection*, Types::UserIdType, QString)),
            this,          SIGNAL(loginUserRequestFromServer(Connection *, Types::UserIdType2, QString)));

    connect(newConnection, SIGNAL(buyStock(Types::UserIdType, Types::StockIdType, Types::AmountType, Types::PriceType)),
            this,          SIGNAL(buyStock(Types::UserIdType, Types::StockIdType, Types::AmountType, Types::PriceType)));

    connect(newConnection, SIGNAL(sellStock(Types::UserIdType, Types::StockIdType, Types::AmountType, Types::PriceType)),
            this,          SIGNAL(sellStock(Types::UserIdType, Types::StockIdType, Types::AmountType, Types::PriceType)));

    connect(newConnection, SIGNAL(getMyStocks(Types::UserIdType)),
            this,          SIGNAL(getMyStocks(Types::UserIdType)));

    connect(newConnection, SIGNAL(getMyOrders(Types::UserIdType)),
            this,          SIGNAL(getMyOrders(Types::UserIdType)));

    connect(newConnection, SIGNAL(getStockInfo(Types::UserIdType, Types::StockIdType)),
            this,          SIGNAL(getStockInfo(Types::UserIdType, Types::StockIdType)));

    connect(newConnection, SIGNAL(cancelOrder(Types::UserIdType, Types::StockIdType)),
            this,          SIGNAL(cancelOrder(Types::UserIdType, Types::StockIdType)));
    //od teraz dopiero zaczniemy przetwarzac wiadomosci z tego polaczenia
    //abysmy nie robili tego zanim polaczylismy sygnaly i sloty
    //bo moglibysmy stracic wiadomosci
    newConnection->start();
}

void Server::disconnectUser(Types::UserIdType userId)
{
    qDebug() << "[Server] Usuwanie użytkownika " << userId
             << "z rejestru aktywnych użytkowników (lol).";
    // To się chyba nie powinno zdarzyć.
    if(_userConnections.remove(userId) == 0)
    {
        qDebug() << "[Server] Próba usunięcia niepołączonego użytkownika.";
    }
}
