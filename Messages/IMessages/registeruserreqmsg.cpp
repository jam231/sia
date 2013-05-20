#include "registeruserreqmsg.h"
#include "utilities.h"


#include <QDataStream>
#include <QRegExp>
#include <QDebug>

RegisterUserReqMsg::RegisterUserReqMsg(QDataStream& in) //: IMessage()
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
        throw InvalidDataInMsg();

    in >> passwordLength;

    if(in.device()->bytesAvailable() != passwordLength)
        throw InvalidDataInMsg();

    QByteArray buffer(passwordLength, Qt::Uninitialized);
    in.readRawData(buffer.data(), passwordLength);
    m_password = QString(buffer);
}

IOMessage::MessageType RegisterUserReqMsg::type() const
{
    return REGISTER_USER_REQ;
}

QString RegisterUserReqMsg::getPassword() const
{
    return m_password;
}
