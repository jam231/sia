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

#include "unrecognizedusermsg.h"
#include "loginuserrespfail.h"
#include "registeruserrespfail.h"
#include <neworder.h>

Connection::Connection(QTcpSocket* socket, QObject *parent) :
    QObject(parent), m_socket(socket), m_userId(NOT_ASSIGNED)//, m_tmpUserId(-1)
{
    m_socket->setParent(this);
}

Connection::~Connection()
{
    delete m_socket;
    qDebug() << "[Connection] Połączenie zostało zerwane.";
}

int Connection::userId() const
{
    return m_userId;
}

bool Connection::isUserAssigned() const
{
    return m_userId != NOT_ASSIGNED;
}
/*
void Connection::setTmpUserId(qint32 tmpUserId)
{
    m_tmpUserId = tmpUserId;
}
*/
void Connection::setUserId(qint32 userId)
{
    m_userId = userId;
}

void Connection::start()
{
    qDebug() << "[Connection] Rozpoczynanie nowego połączenia.";
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(disconect()));
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readData()));
    if(!m_socket->isValid())
    {
        qDebug() << "[Connection] Wykryto błąd" << m_socket->errorString()
                 << " podczas rozpoczynania nowego połączenia.";
        disconect(); //jak połączenie sie zerwało zanim połączyliśmy sloty
        return;
    }
    qDebug() << "[Connection] Ustanowiono nowe połączenie.";
    //tak na wszelki wypadek jakbysmy dostali już jakieś dane zanim zdążyliśmy połączyć sygnały
    readData();
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
    qDebug() << "[Connection] Wiadomośc wysłana.";
    msg.send(m_socket);
    return true;

}

bool Connection::send(NewOrder& msg)
{
    if(m_subscribedStocks.contains(msg.getStockId()))
        return send(static_cast<OMessage&>(msg));
    return false;
}

void Connection::disconect()
{
    qDebug() << "[Connection] Zrywanie połączenia...";
    if(m_userId != NOT_ASSIGNED)
        emit disconnected(m_userId);
    // Usun po tym jak wszystkie sygnaly zostały przetworzone.
    deleteLater();
}

void Connection::readData()
{
    /*
     * Przeczytaj pierwsze 2 bajty, które stanowią długość całej wiadomości.
     * Nastepnie, wczytaj do bufora (?) zadana długość bajtów.
     *
     * Jeżeli dł. wiadomości < 2 to wiadomośc jest wadliwa, zignoruj.
     * Jeżeli wczytano do bufora mniej bajtów niż wynosi wiadomośc
     * to wiadomość jest niepoprawna (corrupted) i zignoruj ją.
     */
    qint16 msgLength = IMessage::getMsgLength(m_socket);

    if(msgLength < 1)
        return;

    QDataStream message(m_socket->read(msgLength));

    IOMessage::MessageType msgType = IMessage::getMsgType(message);

    qDebug() << "[Connection] Id wiadmości:" << msgType;

    switch(msgType)
    {
        case IOMessage::REGISTER_USER_REQ:
        {
            qDebug() << "[Connection] Żądanie rejestracji.";
            if(!isUserAssigned())
            {
                try {
                    RegisterUserReqMsg request(message);
                    emit registerUserRequestFromConnection(this, request.getPassword());
                }catch(const std::exception& e)
                {
                    qDebug() << "[Connection] Złapano wyjątek" << e.what()
                             << "podczas procesu rejestracji.";
                }
            }
            else
            {
                qDebug() << "[Connection] Próba rejestracji przez "\
                            "zalogowanego użytkownika.";
                RegisterUserRespFail response("Zalogowany.");
                send(response);
            }
            return;
        }
        case IOMessage::LOGIN_USER_REQ:
        {
            if(!isUserAssigned())
            {

                qDebug() << "[Connection] Żądanie autoryzacji.";
                try
                {
                    LoginUserReqMsg request(message);
                    qDebug() << "[Connection] Id użytkownika " << request.getUserId();
                    emit loginUserRequestFromConnection(this, request.getUserId(),
                                                        request.getUserPassword());
                }catch(const std::exception& e)
                {
                    qDebug() << "[Connection] Złapano wyjątek" << e.what()
                             << "podczas procesu logowania.";
                }
            }
            else
            {
                qDebug() << "[Connection] Wykryto próbę wielokrotnego"
                         << "logowania przez użytkownika" << m_userId;
                LoginUserRespFail response("Już zalogowany.");
                send(response);
            }
            return;
        }
        case IOMessage::UNDEFINED:
        {
            qDebug() << "[Connection] Otrzymano nieznany typ wiadomości: "
                     << msgType << ".";;
            return;
        }
        default:
        {
            break;
        }
    }

    //jesli nie mamy przypisanego usera do tego polaczenia wysylamy UNRECOGNIZED
    if(!isUserAssigned())
    {
        qDebug() << "[Connection] Nierozpoznano użytkownika.";
        unrecognizedUserMsg Msg;
        Msg.send(m_socket);
        return;
    }

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
                qDebug() << "[Connection] Złapano wyjątek" << e.what()
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
                qDebug() << "[Connection] Złapano wyjątek" << e.what()
                         << "podczas zlecania kupna.";
            }
            break;
        }
        case IOMessage::GET_STOCKS:
            //emit getStocks(m_userId);
            break;
        case IOMessage::SUBSCRIBE_STOCK:
        {
            try
            {
                SubscribeStockMsg msg(message);
                addSubscription(msg.getStockId());
            }catch(const std::exception& e)
            {
                qDebug() << "[Connection] Złapano wyjątek" << e.what()
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
                qDebug() << "[Connection] Złapano wyjątek" << e.what()
                         << "podczas zlecenia usuniecia subskrypcji.";
            }
            break;
        }
        default:
        {
            qDebug() << "[Connection] Nieodpowiedni typ wiadomości -"
                     << "najpewniej użytkownik wysłał wiadomośc przeznaczoną"
                     << "do wysyłania przez serwer.";
            break;
        }
    }
}
