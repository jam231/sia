#include "registeruserreqmsg.h"

#include <QDataStream>
#include <QRegExp>
#include <QDebug>

RegisterUserReqMsg::RegisterUserReqMsg(QIODevice* msg) //: IMessage()
{
    /*
     *  tmpStream >> m_Password
     *  Czytanie QStringa nie zadziała, jeżeli do jego wysyłania
     *  nie został użyty QDataStream - tak wyczytałem na stackoverflow,
     *  i rzeczywiscie nie chciał mi się wczytać --jam231
     */
    QDataStream in(msg);
    in.setByteOrder(QDataStream::BigEndian);
    QByteArray buffer(msg->bytesAvailable(), Qt::Uninitialized);

    in.readRawData(buffer.data(), msg->bytesAvailable());
    m_password = QString(buffer);

    qDebug() << "[RegisterUserReqMsg] Podane hasło:" << m_password;
    if(m_password.length() > 15 || m_password.contains(QRegExp("\\s"))
            || m_password.length() < 5)
    {
        qDebug() << "[RegisterUserReqMsg] Niepoprawne hasło"
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
