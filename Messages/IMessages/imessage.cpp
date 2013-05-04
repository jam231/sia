#include "imessage.h"

#include <QDebug>
#include <QDataStream>

IMessage::IMessage() : IOMessage()
{
}


IOMessage::MessageType IMessage::getMsgType(QIODevice* data)
{
    QDataStream tmpStream(data);
    qint32 tmpType;
    tmpStream >> tmpType;

    return toType(tmpType);
}

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
