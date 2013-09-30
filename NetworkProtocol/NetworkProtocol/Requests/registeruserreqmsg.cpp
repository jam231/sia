#include "registeruserreqmsg.h"

#include <QDebug>

namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

RegisterUserReqMsg::RegisterUserReqMsg(QDataStream& in) : Request()
{
    /*
     *  in >> m_Password
     *  Czytanie QStringa nie zadziała, jeżeli do jego wysyłania
     *  nie został użyty QDataStream - tak wyczytałem na stackoverflow,
     *  i rzeczywiscie nie chciał mi się wczytać
     *                                          --jam231
     */

    Types::MessageLengthType passwordLength;

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

Types::MessageType RegisterUserReqMsg::type() const
{
    return Types::MessageType::REQUEST_REGISTER_USER;
}

QString RegisterUserReqMsg::getPassword() const
{
    return m_password;
}


DTO::Types::MessageLengthType RegisterUserReqMsg::length() const
{
    return sizeof(Types::MessageType) +
           sizeof(Types::MessageLengthType) + m_password.toUtf8().size();
}

const QString RegisterUserReqMsg::getMessageName()
{
    return Request::getMessageName() + " :: RegisterUserReqMsg";
}

}
}
