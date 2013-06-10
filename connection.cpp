#include <QDebug>

#include "connection.h"
#include "utilities.h"

#include "iomessage.h"
#include "imessage.h"
#include "omessage.h"

#include "registeruserreqmsg.h"
#include "loginuserreqmsg.h"
#include "buystockreqmsg.h"
#include "sellstockreqmsg.h"
#include "subscribestockmsg.h"
#include "unsubscribestockmsg.h"
#include "getstockinfomsg.h"

#include "unrecognizedusermsg.h"
#include "loginuserrespfail.h"
#include "registeruserrespfail.h"
#include "order.h"
#include "cancelordermsg.h"

Connection::Connection(QTcpSocket* socket, QObject *parent) :
    QObject(parent), m_socket(socket), m_userId(NOT_ASSIGNED)
{
    m_socket->setParent(this);
}

Connection::~Connection()
{
    delete m_socket;
    qDebug() << "\t\t[Connection] Połączenie zostało zerwane.";
}

int Connection::userId() const
{
    return m_userId;
}

bool Connection::isUserAssigned() const
{
    return m_userId != NOT_ASSIGNED;
}

void Connection::setUserId(qint32 userId)
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
        qDebug() << "\t\t[Connection] Wykryto błąd" << m_socket->errorString()
                 << " podczas rozpoczynania nowego połączenia.";
        disconect(); //jak połączenie sie zerwało zanim połączyliśmy sloty
        return;
    }
    qDebug() << "\t\t[Connection] Ustanowiono nowe połączenie.";
    //tak na wszelki wypadek jakbysmy dostali już jakieś dane zanim zdążyliśmy połączyć sygnały
    processIncomingMessages();
}

void Connection::addSubscription(qint32 stockId)
{
    m_subscribedStocks.insert(stockId);
}

void Connection::dropSubscription(qint32 stockId)
{
    m_subscribedStocks.remove(stockId);
}

bool Connection::send(OMessage& msg)
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

bool Connection::send(OrderMsg& msg)
{
    if(m_subscribedStocks.contains(msg.getStockId()))
    {
        return send(static_cast<OMessage&>(msg));

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
    qint16 msgLength = IMessage::getMsgLength(m_socket);

    if(0 >= msgLength || msgLength > m_socket->bytesAvailable())
        return false;
    // Odrzuc pierwsze dwa bajty
    m_socket->read(2);
    QDataStream message(m_socket->read(msgLength));

    IOMessage::MessageType msgType = IMessage::getMsgType(message);

    //qDebug() << "\t\t[Connection] Id wiadmości:" << msgType;

    switch(msgType)
    {
        case IOMessage::REGISTER_USER_REQ:
        {
            qDebug() << "\t\t[Connection] Żądanie rejestracji.";
            if(!isUserAssigned())
            {
                try {
                    RegisterUserReqMsg request(message);
                    emit registerUserRequestFromConnection(this, request.getPassword());
                }catch(const std::exception& e)
                {
                    qDebug() << "\t\t[Connection] Złapano wyjątek" << e.what()
                             << "podczas procesu rejestracji.";
                }
            }
            else
            {
                qDebug() << "\t\t[Connection] Próba rejestracji przez "\
                            "zalogowanego użytkownika.";
                RegisterUserRespFail response("Zalogowany.");
                send(response);
            }
            return true;
        }
        case IOMessage::LOGIN_USER_REQ:
        {
            if(!isUserAssigned())
            {

                qDebug() << "\t\t[Connection] Żądanie autoryzacji.";
                try
                {
                    LoginUserReqMsg request(message);
                    qDebug() << "\t\t[Connection] Id użytkownika " << request.getUserId();
                    emit loginUserRequestFromConnection(this, request.getUserId(),
                                                        request.getUserPassword());
                }catch(const std::exception& e)
                {
                    qDebug() << "\t\t[Connection] Złapano wyjątek" << e.what()
                             << "podczas procesu logowania.";
                }
            }
            else
            {
                qDebug() << "\t\t[Connection] Wykryto próbę wielokrotnego"
                         << "logowania przez użytkownika" << m_userId;
                LoginUserRespFail response("Już zalogowany.");
                send(response);
            }
            return true;
        }
        case IOMessage::UNDEFINED:
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

    //jesli nie mamy przypisanego usera do tego polaczenia wysylamy UNRECOGNIZED
    if(!isUserAssigned())
    {
        qDebug() << "\t\t[Connection] Nierozpoznano użytkownika.";
        UnrecognizedUserMsg Msg;

        Msg.send(m_socket);
        return true;
    }
    try
    {
    switch(msgType)
    {
        case IOMessage::BUY_STOCK_REQ:
        {
            try
            {
                BuyStockReqMsg msg(message);
                emit buyStock(m_userId, msg.getStockId(),
                              msg.getAmount(), msg.getPrice());
            }catch(const std::exception& e)
            {
                qDebug() << "\t\t[Connection] Złapano wyjątek" << e.what()
                         << "podczas zlecania kupna.";
            }
            break;
        }
        case IOMessage::SELL_STOCK_REQ:
        {
            try
            {
                SellStockReqMsg msg(message);
                emit sellStock(m_userId, msg.getStockId(),
                              msg.getAmount(), msg.getPrice());
            }catch(const std::exception& e)
            {
                qDebug() << "\t\t[Connection] Złapano wyjątek" << e.what()
                         << "podczas zlecania sprzedazy.";
            }
            break;
        }
        case IOMessage::CANCEL_ORDER_REQ:
        {
            try
            {
                CancelOrderMsg msg(message);
                emit cancelOrder(m_userId, msg.getOrderId());
            }catch(const std::exception& e)
            {
                qDebug() << "\t\t[Connection] Złapano wyjątek" << e.what()
                         << "podczas anulowania transakcji.";
            }
            break;
        }
        case IOMessage::GET_MY_STOCKS:
            emit getMyStocks(m_userId);
            break;
        case IOMessage::GET_MY_ORDERS:
            emit getMyOrders(m_userId);
            break;
        case IOMessage::GET_STOCK_INFO:
        {
            GetStockInfoMsg msg(message);

            emit getStockInfo(m_userId, msg.getStockId());
            break;
        }
        case IOMessage::SUBSCRIBE_STOCK:
        {
            try
            {
                SubscribeStockMsg msg(message);
                addSubscription(msg.getStockId());
            }catch(const std::exception& e)
            {
                qDebug() << "\t\t[Connection] Złapano wyjątek" << e.what()
                         << "podczas zlecania subskypcji.";
            }
            break;
        }
        case IOMessage::UNSUBSCRIBE_STOCK:
        {
            try
            {
                UnsubscribeStockMsg msg(message);
                dropSubscription(msg.getStockId());
            }catch(const std::exception& e)
            {
                qDebug() << "\t\t[Connection] Złapano wyjątek" << e.what()
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
        qDebug() << "\t\t[Connection] STALO SIE COS NIESPODZIEWANEGO !:" << e.what();
    }

    return true;
}

void Connection::processIncomingMessages()
{
    while(processMessage())
    {}
}
