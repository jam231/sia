#include "unrecognizedusermsg.h"

#include <QDataStream>

unrecognizedUserMsg::unrecognizedUserMsg()
{}

IOMessage::MessageType unrecognizedUserMsg::type() const
{
    return UNRECOGNIZED_USER;
}

qint32 unrecognizedUserMsg::length() const
{
    //8 bajtów na typ i dlugosc
    return 8;
}


void unrecognizedUserMsg::send(QIODevice* connection)
{
    QDataStream tmpStream(connection);
    tmpStream<<length();
    tmpStream<<static_cast<qint32>(type());
}
