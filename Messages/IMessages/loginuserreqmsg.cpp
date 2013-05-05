#include "loginuserreqmsg.h"
#include "utilities.h"

#include <QDataStream>
#include <QRegExp>
#include <QDebug>

LoginUserReqMsg::LoginUserReqMsg(QIODevice* msg)
{
    QDataStream in(msg);
    in.setByteOrder(QDataStream::BigEndian);

    in >> m_userId;
    QByteArray buffer(msg->bytesAvailable(), Qt::Uninitialized);

    in.readRawData(buffer.data(), msg->bytesAvailable());
    m_password = QString(buffer);

    /*
     * Być może nie jest to miejsce na sprawdzenie zgodności
     * hasła z ustalonymi kryteriami.
     * Być może nawet jest to zbędne i nic nie daje oprócz zaciemniania
     * kodu, ale białych znaków w haśle nie chciałbym mieć -> SQL injection.
     */
    if(m_password.length() > 15 || m_password.contains(QRegExp("\\s"))
            || m_password.length() < 5)
    {
        qDebug() << "[LoginUserReqMsg] Niepoprawne hasło."
                 << m_password;
        throw InvalidPasswordError();
    }

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
