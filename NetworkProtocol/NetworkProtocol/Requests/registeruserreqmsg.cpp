#include "registeruserreqmsg.h"

#include <QDebug>

namespace NetworkProtocol
{
namespace Requests
{

RegisterUserReqMsg::RegisterUserReqMsg(QDataStream& in) : Request()
{
    /*
     *  tmpStream >> m_Password
     *  Czytanie QStringa nie zadziała, jeżeli do jego wysyłania
     *  nie został użyty QDataStream - tak wyczytałem na stackoverflow,
     *  i rzeczywiscie nie chciał mi się wczytać --jam231
     */
    // Domyślnie BigEndian
    qint16 passwordLength;

    if(in.device()->bytesAvailable() < sizeof(passwordLength))
    {
        qDebug() << "[" << getMessageName() << "] Not enough bytes for password length.";
        throw InvalidRequest();
    }

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

Message::MessageType RegisterUserReqMsg::type() const
{
    return REGISTER_USER_REQ;
}

QString RegisterUserReqMsg::getPassword() const
{
    return m_password;
}


qint16 RegisterUserReqMsg::length() const
{
    return sizeof(MessageType) + sizeof(qint16) + m_password.toUtf8().size();
}

}
}
