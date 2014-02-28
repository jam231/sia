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
     *  in >> _Password
     *  Czytanie QStringa nie zadziała, jeżeli do jego wysyłania
     *  nie został użyty QDataStream - tak wyczytałem na stackoverflow,
     *  i rzeczywiscie nie chciał mi się wczytać
     *                                          --jam231
     */

    Types::Message::MessageLengthType passwordLength;

    if(in.device()->bytesAvailable() < sizeof(passwordLength))
    {
        qWarning() <<"[] Zbyt mało bajtów by odczytać długośc hasła.\n"
                   << "Oczekiwano" << sizeof(passwordLength) << "bajtów.\n"
                   << "Liczba bajtów dostępnych w buforze:"
                   << in.device()->bytesAvailable();
        throw InvalidRequest();
    }

    in >> passwordLength;

    if(in.device()->bytesAvailable() != passwordLength)
    {
        qWarning() <<"[] Niepoprawna długość hasła.\n"
                   << "Oczekiwano" << length() << "bajtów.\n"
                   << "Liczba bajtów dostępnych w buforze:"
                   << in.device()->bytesAvailable();
        throw InvalidRequest();
    }

    QByteArray buffer(passwordLength, Qt::Uninitialized);
    in.readRawData(buffer.data(), passwordLength);
    _password = QString(buffer);
}

Types::Message::MessageType RegisterUser::type() const
{
    return Types::Message::MessageType::REQUEST_REGISTER_USER;
}

QString RegisterUser::getPassword() const
{
    return _password;
}


DTO::Types::Message::MessageLengthType RegisterUser::length() const
{
    return sizeof(Types::Message::MessageType) +
           sizeof(Types::Message::MessageLengthType) + _password.toUtf8().size();
}

}
}
