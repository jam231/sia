#include "registeruserreqmsg.h"

#include <QDataStream>
#include <QRegExp>
#include <QDebug>

RegisterUserReqMsg::RegisterUserReqMsg(QIODevice* msg) : IMessage()
{
    /*
     *  tmpStream >> m_Password
     *  Won't work if QString hasn't been put with QDataStream
     *
     */
    QDataStream out(msg);
    out.setByteOrder(QDataStream::BigEndian);
    QByteArray buffer(msg->bytesAvailable(), Qt::Uninitialized);

    out.readRawData(buffer.data(), msg->bytesAvailable());
    m_password = QString(buffer);

    qDebug() << "[RegisterUserReqMsg]" << m_password;
    if(m_password.length() > 15 || m_password.contains(QRegExp("\\s"))
            || m_password.length() < 1)
    {
        qDebug() << "[RegisterUserReqMsg] Invalid password:"
                 << m_password;
        throw InvalidPasswordError();
    }
}

IOMessage::MessageType RegisterUserReqMsg::type() const
{
    return REGISTER_USER_REQ;
}

QString RegisterUserReqMsg::getPassword() const
{
    return m_password;
}
