#include "loginuserreqmsg.h"

#include <QRegExp>
#include <QDebug>


namespace NetworkProtocol {

LoginUserReqMsg::LoginUserReqMsg(QDataStream &in)
{
    // Domyślnie BigEndian
    qint16 passwordLength;

    if(in.device()->bytesAvailable() < (sizeof(m_userId) + sizeof(passwordLength)))
        throw InvalidDataInMsg();

    in >> m_userId;
    in >> passwordLength;

    if(in.device()->bytesAvailable() != passwordLength)
        throw InvalidDataInMsg();

    QByteArray buffer(passwordLength, Qt::Uninitialized);

    in.readRawData(buffer.data(), passwordLength);
    m_password = QString(buffer);
}

IOMessage::MessageType LoginUserReqMsg::type() const
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

}
