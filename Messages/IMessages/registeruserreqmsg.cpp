#include "registeruserreqmsg.h"

#include <QDataStream>

RegisterUserReqMsg::RegisterUserReqMsg(QIODevice* msg) : IMessage()
{
    QDataStream tmpStream;
    tmpStream>>m_cash;
}

IOMessage::MessageType RegisterUserReqMsg::type() const
{
    return REGISTER_USER_REQ;
}

double RegisterUserReqMsg::cash() const
{
    return m_cash;
}
