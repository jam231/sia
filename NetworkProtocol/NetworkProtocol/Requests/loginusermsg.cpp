#include "loginusermsg.h"

#include <QRegExp>
#include <QDebug>


namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

LoginUser::LoginUser(QDataStream &in) : Request() // Pakiet może mieć różną długość w zależności
{                                                 // od długości hasła.
    // Domyślnie BigEndian
    Types::Message::MessageLengthType passwordLength;

    if(in.device()->bytesAvailable() < (sizeof(m_userId) + sizeof(passwordLength)))
    {
        qWarning() <<"[" << getMessageName() << "] Zbyt mało bajtów by odczytać długośc hasła.\n"
                  << "Oczekiwano" << sizeof(passwordLength) << "bajtów.\n"
                   << "Liczba bajtów dostępnych w buforze:"
                   << in.device()->bytesAvailable();
        throw InvalidRequest();
    }
    in >> m_userId;
    in >> passwordLength;

    if(in.device()->bytesAvailable() != passwordLength)
    {
        qWarning() <<"[" << getMessageName() << "] Niepoprawna długość hasła.\n"
                   << "Oczekiwano" << length() << "bajtów.\n"
                   << "Liczba bajtów dostępnych w buforze:"
                   << in.device()->bytesAvailable();
        throw InvalidRequest();
    }
    QByteArray buffer(passwordLength, Qt::Uninitialized);

    in.readRawData(buffer.data(), passwordLength);
    m_password = QString(buffer);
}

Types::Message::MessageType LoginUser::type() const
{
    return Types::Message::MessageType::REQUEST_LOGIN_USER;
}

Types::UserIdType LoginUser::getUserId() const
{
    return m_userId;
}

QString LoginUser::getUserPassword() const
{
    return m_password;
}

Types::Message::MessageLengthType LoginUser::length() const
{
    return sizeof(Types::Message::MessageType) +
           sizeof(m_userId) + sizeof(Types::Message::MessageLengthType) + m_password.toUtf8().size();
}

const QString LoginUser::getMessageName()
{
    return Request::getMessageName() + " :: LoginUser";
}


}
}
