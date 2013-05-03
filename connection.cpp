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
    QObject(parent), m_socket(socket), m_userId(-1), m_tmpUserId(-1)
{
    m_socket->setParent(this);
}

int Connection::userId() const
{
    return m_userId;
}

bool Connection::isUserAssigned() const
{
    return m_userId != -1;
}

void Connection::setTmpUserId(qint32 tmpUserId)
{
    m_tmpUserId = tmpUserId;
}

void Connection::start()
{
    qDebug() << "[Connection] Starting new connection...";
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(disconect()));
    if(!m_socket->isValid())
    {
        qDebug() << "[Connection] Error " << m_socket->errorString()
                 << " occured while establishing new connection.";
        disconect(); //jak połączenie sie zerwało zanim połączyliśmy sloty
        return;
    }
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readData()));
    qDebug() << "[Connection] Connection has been established.";
    //tak na wszelki wypadek jakbysmy dostali już jakieś dane zanim zdążyliśmy połączyć sygnały
    readData();
}

bool Connection::send(OMessage& msg)
{
    if(!isUserAssigned())
        return false;

    msg.send(m_socket);
    return true;
}

void Connection::disconect()
{
    if(m_userId >= 0)
        emit disconnected(m_userId, false);
    if(m_tmpUserId >= 0)
        emit disconnected(m_tmpUserId, true);

    // delete object after all signals have been processed.
    deleteLater();
}


void Connection::readData()
{
    //otrzymaliśmy za mało danych, aby przetworzyć wiadomosc w calosci
    if(!IMessage::isEnoughData(m_socket)) {
        qDebug() << "[Connection] Not enough data for a valid message.";
        return;
    }
    IOMessage::MessageType msgType = IMessage::getMsgType(m_socket);

    qDebug() << "[Connection] message id:" << msgType;

    switch(msgType)
    {
        case IOMessage::REGISTER_USER_REQ:
        {
            qDebug() << "[Connection] Registration request.";
            try {
                RegisterUserReqMsg Msg(m_socket);
                emit registerReq(this, Msg.getPassword());
            }catch(const std::exception& e)
            {
                qDebug() << "[Connection] Exception" << e.what()
                         << "has been thrown.";
            }catch(...)
            {
                qDebug() << "[Connection] Unknown exception occurred"
                         << "while processing register request.";
            }
            return;
        }
        case IOMessage::LOGIN_USER:
        {
            qDebug() << "[Connection] Login request.";
            LoginUserMsg Msg(m_socket);
            qDebug() << "[Connection] User id: " << m_userId;
            m_userId = Msg.userId();
            emit assigned(this, m_userId);
            return;
        }
        /*
         *  Added because, when I get undefined messsage I want
         *  to know that I got undefined message and not
         *  user not assigned !
         */
        case IOMessage::UNDEFINED:
        {
            qDebug() << "[Connection] Received unrecognised message type: "
                     << msgType << ".";
            return;
        }
        default:
        {
            qDebug() << "[Connection] Fatal error 1 occured while message handling.";
            return;
        }
    }

    //jesli nie mamy przypisanego usera do tego polaczenia wysylamy UNRECOGNIZED
    if(!isUserAssigned())
    {
        qDebug() << "[Connection] User has not been recognised.";
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
            qDebug() << "[Connection] Fatal error 2 occured while message handling.";
            break;
        }
    }
}
