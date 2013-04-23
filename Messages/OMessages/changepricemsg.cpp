#include "changepricemsg.h"

#include <QDataStream>

ChangePriceMsg::ChangePriceMsg(const Stock& stock) : OMessage(), m_stock(stock)
{
}

qint32 ChangePriceMsg::length() const
{
    //8 bajt�w na typ i dlugosc
    return 8 + sizeof(ChangePriceMsg);
}

IOMessage::MessageType ChangePriceMsg::type() const
{
    return CHANGE_PRICE;
}

void ChangePriceMsg::send(QIODevice* connection)
{
    QDataStream tmpStream(connection);
    tmpStream<<length();
    tmpStream<<static_cast<qint32>(type());
    tmpStream<<m_stock;
}
