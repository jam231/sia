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
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(disconect()));
    if(!m_socket->isValid())
    {
        disconect(); //jak połączenie sie zerwało zanim połączyliśmy sloty
        return;
    }
    connect(m_socket, SIGNAL(readyRead()), this, SLOT(readData()));

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

    deleteLater();
}

void Connection::readData()
{
    //otrzymaliśmy za mało danych, aby przetworzyć wiadomosc w calosci
    if(!IMessage::isEnoughData(m_socket))
        return;

    IOMessage::MessageType msgType = IMessage::getMsgType(m_socket);

    switch(msgType)
    {
        case IOMessage::REGISTER_USER_REQ:
        {
            RegisterUserReqMsg Msg(m_socket);
            emit registerReq(this, Msg.cash());
            return;
        }
        case IOMessage::LOGIN_USER:
        {
            LoginUserMsg Msg(m_socket);

            m_userId = Msg.userId();
            emit assigned(this, m_userId);
            return;
        }
    };

    //jesli nie mamy przypisanego usera do tego polaczenia wysylamy UNRECOGNIZED
    if(!isUserAssigned())
    {
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
    }

}
