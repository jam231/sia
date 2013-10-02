#include "loginuserreqmsg.h"

#include <QRegExp>
#include <QDebug>


namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

LoginUserReqMsg::LoginUserReqMsg(QDataStream &in) : Request()
{
    // Domyślnie BigEndian
    Types::MessageLengthType passwordLength;

    if(in.device()->bytesAvailable() < (sizeof(m_userId) + sizeof(passwordLength)))
    {
        qDebug() << "[" << getMessageName() << "] Invalid password length in request.\n"
                 << "Is" << in.device()->bytesAvailable()
                 << "| should be" << passwordLength;
        throw InvalidRequest();
    }
    in >> m_userId;
    in >> passwordLength;

    if(in.device()->bytesAvailable() != passwordLength)
    {
        qDebug() << "[" << getMessageName() << "] Invalid password length in request.\n"
                 << "Is" << in.device()->bytesAvailable()
                 << "| should be" << passwordLength;
        throw InvalidRequest();
    }
    QByteArray buffer(passwordLength, Qt::Uninitialized);

    in.readRawData(buffer.data(), passwordLength);
    m_password = QString(buffer);
}

Types::Message::MessageType LoginUserReqMsg::type() const
{
    return Types::Message::MessageType::LOGIN_USER_REQ;
}

Types::UserIdType LoginUserReqMsg::getUserId() const
{
    return m_userId;
}

QString LoginUserReqMsg::getUserPassword() const
{
    return m_password;
}

Types::MessageLengthType LoginUserReqMsg::length() const
{
    return sizeof(Types::Message::MessageType) +
           sizeof(m_userId) + sizeof(Types::MessageLengthType) + m_password.toUtf8().size();
}

const QString LoginUserReqMsg::getMessageName()
{
    return Request::getMessageName() + " :: LoginUserReqMsg";
}


}
}
