#include <QDebug>

#include "connection.h"

#include "iomessage.h"
#include "imessage.h"
#include "omessage.h"

#include "registeruserreqmsg.h"
#include "loginusermsg.h"
#include "buystockreqmsg.h"
#include "sellstockreqmsg.h"
#include "subscribestockmsg.h"
#include "unsubscribestockmsg.h"

#include "unrecognizedusermsg.h"

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

bool Connection::send(OMessage& msg)
{
    /* Po co to jest? Jakas proba sprawdzenia, czy ten uzytkownik jest
     * jeszcze podlaczony i oplaca mu sie wysylac pakiet ?
     *
     *
     * if(!isUserAssigned())
     *   return false;
     */
    msg.send(m_socket);
    // Assign user.
    return true;

}

void Connection::disconect()
{
    qDebug() << "[Connection] Zrywanie połączenia...";
    if(m_userId != NOT_ASSIGNED)
        emit disconnected(m_userId);
   // if(m_tmpUserId >= 0)
   //     emit disconnected(m_tmpUserId, true);

    // Usun po tym jak wszystkie sygnaly zostały przetworzone.
    deleteLater();
}


void Connection::readData()
{
    IOMessage::MessageType msgType = IMessage::getMsgType(m_socket);

    qDebug() << "[Connection] Id wiadmości:" << msgType;

    switch(msgType)
    {
        case IOMessage::REGISTER_USER_REQ:
        {
            qDebug() << "[Connection] Żądanie rejestracji.";
            try {
                RegisterUserReqMsg Msg(m_socket);
                emit registerUserRequestFromConnection(this, Msg.getPassword());
            }catch(const std::exception& e)
            {
                qDebug() << "[Connection] Złapano wyjątek " << e.what();
            }catch(...)
            {
                qDebug() << "[Connection] Złapano nieznany wyjątek " \
                            "podczas procesu rejestracji.";
            }
            return;
        }
        case IOMessage::LOGIN_USER:
        {
            qDebug() << "[Connection] Żądanie autoryzacji.";
            LoginUserMsg Msg(m_socket);
            qDebug() << "[Connection] Id użytkownika " << m_userId;
            m_userId = Msg.userId();
            emit assigned(this, m_userId);
            return;
        }
        case IOMessage::UNDEFINED:
        {
            qDebug() << "[Connection] Otrzymano nieznany typ wiadomości: "
                     << msgType << ".";
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
            BuyStockReqMsg Msg(m_socket);
            emit buyStock(m_userId, Msg.offer());
            break;
        }
        case IOMessage::SELL_STOCK_REQ:
        {
            SellStockReqMsg Msg(m_socket);
            emit sellStock(m_userId, Msg.offer());
            break;
        }
        case IOMessage::GET_STOCKS:
            emit getStocks(m_userId);
            break;
        case IOMessage::SUBSCRIBE_STOCK:
        {
            SubscribeStockMsg Msg(m_socket);
            emit subscribeStock(m_userId, Msg.stockId());
            break;
        }
        case IOMessage::UNSUBSCRIBE_STOCK:
        {
            UnsubscribeStockMsg Msg(m_socket);
            emit unsubscribeStock(m_userId, Msg.stockId());
            break;
        }
        default:
        {
            qDebug() << "[Connection] Zdarzyło się coś nie możliwego.";
            break;
        }
    }
}
