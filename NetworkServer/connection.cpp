#include <QDebug>

#include "connection.h"
#include "Utils/utils.h"

#include <Requests/registerusermsg.h>
#include <Requests/loginusermsg.h>
#include <Requests/buystockmsg.h>
#include <Requests/sellstockmsg.h>
#include <Requests/subscribestockmsg.h>
#include <Requests/unsubscribestockmsg.h>
#include <Requests/getstockinfomsg.h>
#include <Requests/cancelordermsg.h>

#include <Responses/failuremsg.h>
#include <DataTransferObjects/order.h>


using namespace NetworkProtocol;
using namespace NetworkProtocol::Requests;
using namespace NetworkProtocol::Responses;

using namespace DTO;

Connection::Connection(QTcpSocket* socket, QObject *parent) :
    QObject(parent), m_socket(socket), m_userId(NOT_ASSIGNED)
{
    m_socket->setParent(this);
}

Connection::~Connection()
{
    delete m_socket;
    qWarning() << "\t\t[Connection] Połączenie zostało zerwane.";
}

Types::UserIdType Connection::userId() const
{
    return m_userId;
}

bool Connection::isUserAssigned() const
{
    return m_userId != NOT_ASSIGNED;
}

void Connection::setUserId(Types::UserIdType userId)
{
    m_userId = userId;
}
void Connection::start()
{
    qDebug() << "\t\t[Connection] Rozpoczynanie nowego połączenia.";

    connect(m_socket, SIGNAL(disconnected()),
            this,     SLOT(disconect()));

    connect(m_socket, SIGNAL(readyRead()),
            this,     SLOT(processIncomingMessages()));

    if(!m_socket->isValid())
    {
        qCritical() << "\t\t[Connection] Wykryto błąd" << m_socket->errorString()
                    << " podczas rozpoczynania nowego połączenia.";
        disconect(); //jak połączenie sie zerwało zanim połączyliśmy sloty
        return;
    }
    qDebug() << "\t\t[Connection] Ustanowiono nowe połączenie.";
    //tak na wszelki wypadek jakbysmy dostali już jakieś dane zanim zdążyliśmy połączyć sygnały
    processIncomingMessages();
}

void Connection::addSubscription(Types::StockIdType stockId)
{
    m_subscribedStocks.insert(stockId);
}

void Connection::dropSubscription(Types::StockIdType stockId)
{
    m_subscribedStocks.remove(stockId);
}

bool Connection::send(Response& msg)
{
    /* Po co to jest? Jakas proba sprawdzenia, czy ten uzytkownik jest
     * jeszcze podlaczony i oplaca mu sie wysylac pakiet ?
     *
     *
     * if(!isUserAssigned())
     *   return false;
     */
    qDebug() << "\t\t[Connection] Wiadomośc: " << msg.type() << " wysłana.";
    msg.send(m_socket);
    return true;

}

bool Connection::send(ShowOrder msg)
{
    if(m_subscribedStocks.contains(msg.getOrder().getStockId()))
    {
        return send(static_cast<Response&>(msg));

    }
    return false;
}

void Connection::disconect()
{
    qDebug() << "\t\t[Connection] Zrywanie połączenia...";
    if(m_userId != NOT_ASSIGNED)
        emit disconnected(m_userId);
    // Usun po tym jak wszystkie sygnaly zostały przetworzone.
    deleteLater();
}


/*
 *  true <- jezeli bylo wystarczajaca ilosc bajtow by przeczytac wiadomosc
 *  false <- jezeli nie ma wystarczajacej liczby bajtow do przeczytania wiadomosci
 *           (W tym wypadku zawartosc bufora zostaje nienaruszona !)
 */
bool Connection::processMessage()
{
    Types::Message::MessageLengthType msgLength = Request::getMessageLength(m_socket);

    if(0 >= msgLength || msgLength > m_socket->bytesAvailable())
        return false;

    QDataStream message(m_socket->read(msgLength));

    m_socket->read(sizeof(Types::Message::MessageLengthType));

    Types::Message::MessageType msgType = Request::getType(message);

    //qDebug() << "\t\t[Connection] Id wiadmości:" << msgType;

    switch(msgType)
    {
        case Types::Message::MessageType::REQUEST_REGISTER_USER:
        {
            qDebug() << "\t\t[Connection] Żądanie rejestracji.";
            if(!isUserAssigned())
            {
                try {
                    RegisterUser request(message);
                    emit registerUserRequestFromConnection(this, request.getPassword());
                }catch(const std::exception& e)
                {
                    qWarning() << "\t\t[Connection] Złapano wyjątek" << e.what()
                               << "podczas procesu rejestracji.";
                }
            }
            else
            {
                qWarning() << "\t\t[Connection] Próba rejestracji przez "\
                             "zalogowanego użytkownika.";
                Failure response(Types::Failure::CANNOT_REGISTER_WHEN_LOGGED);
                send(response);
            }
            return true;
        }
        case Types::Message::MessageType::REQUEST_LOGIN_USER:
        {
            if(!isUserAssigned())
            {

                qDebug() << "\t\t[Connection] Żądanie autoryzacji.";
                try
                {
                    LoginUser request(message);
                    qDebug() << "\t\t[Connection] Id użytkownika " << request.getUserId();
                    emit loginUserRequestFromConnection(this, request.getUserId(),
                                                        request.getUserPassword());
                }catch(const std::exception& e)
                {
                    qWarning() << "\t\t[Connection] Złapano wyjątek" << e.what()
                               << "podczas procesu logowania.";
                }
            }
            else
            {
                qWarning() << "\t\t[Connection] Wykryto próbę wielokrotnego"
                           << "logowania przez użytkownika" << m_userId;
                Failure response(Types::Failure::ALREADY_LOGGED);
                send(response);
            }
            return true;
        }
        case Types::Message::MessageType::MESSAGE_UNDEFINED:
        {
            qDebug() << "\t\t[Connection] Otrzymano nieznany typ wiadomości: "
                     << msgType << ".";;
            return true;
        }
        default:
        {
            break;
        }
    }

    //jesli nie mamy przypisanego usera do tego polaczenia wysylamy FAILUE::NOT_LOGGED
    if(!isUserAssigned())
    {
        qDebug() << "\t\t[Connection] Nierozpoznano użytkownika.";
        Failure response(Types::Failure::NOT_LOGGED);
        send(response);
        return true;
    }
    try
    {
    switch(msgType)
    {
        case Types::Message::MessageType::REQUEST_BUY_STOCK_ORDER:
        {
            try
            {
                BuyStock msg(message);
                emit buyStock(m_userId, msg.getStockId(),
                              msg.getAmount(), msg.getPrice());
            }catch(const std::exception& e)
            {
                qWarning() << "\t\t[Connection] Złapano wyjątek" << e.what()
                           << "podczas zlecania kupna.";
            }
            break;
        }
        case Types::Message::MessageType::REQUEST_SELL_STOCK_ORDER:
        {
            try
            {
                SellStock msg(message);
                emit sellStock(m_userId, msg.getStockId(),
                              msg.getAmount(), msg.getPrice());
            }catch(const std::exception& e)
            {
                qWarning() << "\t\t[Connection] Złapano wyjątek" << e.what()
                           << "podczas zlecania sprzedazy.";
            }
            break;
        }
        case Types::Message::MessageType::REQUEST_CANCEL_ORDER:
        {
            try
            {
                CancelOrder msg(message);
                emit cancelOrder(m_userId, msg.getOrderId());
            }catch(const std::exception& e)
            {
                qWarning() << "\t\t[Connection] Złapano wyjątek" << e.what()
                           << "podczas anulowania transakcji.";
            }
            break;
        }
        case Types::Message::MessageType::REQUEST_GET_MY_STOCKS:
            emit getMyStocks(m_userId);
            break;
        case Types::Message::MessageType::REQUEST_GET_MY_ORDERS:
            emit getMyOrders(m_userId);
            break;
        case Types::Message::MessageType::REQUEST_GET_STOCK_INFO:
        {
            GetStockInfo msg(message);

            emit getStockInfo(m_userId, msg.getStockId());
            break;
        }
        case Types::Message::MessageType::REQUEST_SUBSCRIBE_STOCK:
        {
            try
            {
                SubscribeStock msg(message);
                addSubscription(msg.getStockId());
            }catch(const std::exception& e)
            {
                qWarning() << "\t\t[Connection] Złapano wyjątek" << e.what()
                           << "podczas zlecania subskypcji.";
            }
            break;
        }
        case Types::Message::MessageType::REQUEST_UNSUBSCRIBE_STOCK:
        {
            try
            {
                UnsubscribeStock msg(message);
                dropSubscription(msg.getStockId());
            }catch(const std::exception& e)
            {
                qWarning() << "\t\t[Connection] Złapano wyjątek" << e.what()
                           << "podczas zlecenia usuniecia subskrypcji.";
            }
            break;
        }
        default:
        {
            qDebug() << "\t\t[Connection] Nieodpowiedni typ wiadomości -"
                     << "najpewniej użytkownik wysłał wiadomośc przeznaczoną"
                     << "do wysyłania przez serwer.";
            break;
        }
    }
    }catch(const std::exception& e)
    {
        qCritical() << "\t\t[Connection] STALO SIE COS NIESPODZIEWANEGO !:" << e.what();
    }

    return true;
}

void Connection::processIncomingMessages()
{
    while(processMessage())
    {}
}
