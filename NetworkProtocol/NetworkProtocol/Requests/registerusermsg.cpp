#include "registerusermsg.h"

#include <QDebug>

namespace NetworkProtocol
{
namespace Requests
{

using namespace DTO;

RegisterUser::RegisterUser(QDataStream& in) : Request() // Pakiet może mieć różną długość w zależności
{                                                       // od długości hasła.
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
        qWarning() <<"[" << getMessageName() << "] Zbyt mało bajtów by odczytać długośc hasła.\n"
                   << "Oczekiwano" << sizeof(passwordLength) << "bajtów.\n"
                   << "Liczba bajtów dostępnych w buforze:"
                   << in.device()->bytesAvailable();
        throw InvalidRequest();
    }

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

Types::Message::MessageType RegisterUser::type() const
{
    return Types::Message::MessageType::REQUEST_REGISTER_USER;
}

QString RegisterUser::getPassword() const
{
    return m_password;
}


DTO::Types::MessageLengthType RegisterUser::length() const
{
    return sizeof(Types::Message::MessageType) +
           sizeof(Types::MessageLengthType) + m_password.toUtf8().size();
}

const QString RegisterUser::getMessageName()
{
    return Request::getMessageName() + " :: RegisterUser";
}

}
}
