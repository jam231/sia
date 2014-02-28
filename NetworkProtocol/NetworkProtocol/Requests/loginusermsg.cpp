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

    if(in.device()->bytesAvailable() < (sizeof(_userId) + sizeof(passwordLength)))
    {
        qWarning() <<"[] Zbyt mało bajtów by odczytać długośc hasła.\n"
                  << "Oczekiwano" << sizeof(passwordLength) << "bajtów.\n"
                   << "Liczba bajtów dostępnych w buforze:"
                   << in.device()->bytesAvailable();
        throw InvalidRequest();
    }
    in >> _userId;
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

Types::Message::MessageType LoginUser::type() const
{
    return Types::Message::MessageType::REQUEST_LOGIN_USER;
}

Types::UserIdType LoginUser::getUserId() const
{
    return _userId;
}

QString LoginUser::getUserPassword() const
{
    return _password;
}

Types::Message::MessageLengthType LoginUser::length() const
{
    return sizeof(Types::Message::MessageType) +
           sizeof(_userId) + sizeof(Types::Message::MessageLengthType) + _password.toUtf8().size();
}


}
}
