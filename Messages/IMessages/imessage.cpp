#include "imessage.h"

#include <QDebug>
#include <QDataStream>


IMessage::IMessage() : IOMessage()
{
}


IOMessage::MessageType IMessage::getMsgType(QIODevice* data)
{
    if(data->bytesAvailable() > 0)
    {
        qint8 msgType;
        QDataStream out(data);
        /* To właściwie jest zbędne bo endianess specyfikuje kolejność
         * bajtów, a tu mamy tylko jeden jednakże, gdyby zaszły jakieś
         * zmiany warto o tym pamiętać.
         */

        out.setByteOrder(QDataStream::BigEndian);

        out >> msgType;

        return toType(msgType);
    }
    return IOMessage::UNDEFINED;
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
