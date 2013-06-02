#include "listofstocksmsg.h"

#include <QDataStream>

ListOfStocksMsg::ListOfStocksMsg() : OMessage(), m_stocks()
{
}


ListOfStocksMsg::ListOfStocksMsg(const QVector<Stock>& stocks) : OMessage(), m_stocks(stocks)
{
}

qint16 ListOfStocksMsg::length() const
{
    //8 bajtow na typ + dlugosc + 4 bajty na ilosc akcji
    return 8 + 4 + m_stocks.size() * sizeof(Stock);
}

IOMessage::MessageType ListOfStocksMsg::type() const
{
    return LIST_OF_STOCKS;
}

void ListOfStocksMsg::send(QIODevice* connection)
{
    QDataStream tmpStream(connection);
    tmpStream<<length();
    tmpStream<<static_cast<qint32>(type());
    tmpStream<<m_stocks;
}

void ListOfStocksMsg::addStock(const Stock& stock)
{
    m_stocks.push_back(stock);
}

