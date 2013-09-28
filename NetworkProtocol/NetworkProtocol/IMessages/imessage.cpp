#include "imessage.h"

#include <QDebug>


namespace NetworkProtocol {

IMessage::IMessage() : IOMessage()
{
}

qint16 IMessage::getMsgLength(QIODevice* data)
{
    if(data->bytesAvailable() < 2)
        return -1;

    qint16 msgLength;

    QDataStream in(data->peek(2));
    in >> msgLength;

    return msgLength;
}

IOMessage::MessageType IMessage::getMsgType(QDataStream& in)
{
    if(in.device()->bytesAvailable() < 1)
        return IOMessage::UNDEFINED;

    //qDebug() << "Wiadomośc jest w porządku.";

    qint8 msgType;
    in >> msgType;
    return toType(msgType);
    /*if(data->bytesAvailable() > 0)
    {
        qint8 msgType;
        // Domyślnie BigEndian
        QDataStream out(data);

        out >> msgType;
        return toType(msgType);
    }
    return IOMessage::UNDEFINED; */
}


/*
bool IMessage::isEnoughData(QIODevice* data)
{

    if(data->bytesAvailable() < 4)
    {
        qDebug() << "[IMessage] Dostępne mniej niż 4 bajty."
                 << "Liczba dostępnych bajtów:" << data->bytesAvailable();
        return false;
    }
    QDataStream out(data->peek(4));
    out.setByteOrder(QDataStream::BigEndian);
    qint32 length;
    out >> length;
    if(data->bytesAvailable() < length) {
        qDebug() << "[IMessage] Liczba dostępnych bajtów mniejsza niż" << length
                 << "Liczba dostępnych bajtów" << data->bytesAvailable();

        return false;
    }
    data->read(4);
    return true;

}
*/

}
