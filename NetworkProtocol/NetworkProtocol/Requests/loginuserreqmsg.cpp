#include "loginuserreqmsg.h"

#include <QRegExp>
#include <QDebug>


namespace NetworkProtocol
{
namespace Requests
{

LoginUserReqMsg::LoginUserReqMsg(QDataStream &in) : Request()
{
    // Domyślnie BigEndian
    qint16 passwordLength;

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

Message::MessageType LoginUserReqMsg::type() const
{
    return LOGIN_USER_REQ;
}

qint32 LoginUserReqMsg::getUserId() const
{
    return m_userId;
}

QString LoginUserReqMsg::getUserPassword() const
{
    return m_password;
}

qint16 LoginUserReqMsg::length() const
{
    return sizeof(MessageType) + sizeof(m_userId) +
           sizeof(qint16) + m_password.toUtf8().size();
}

}
}
