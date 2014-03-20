#ifndef UTILITY_H
#define UTILITY_H

#include <exception>

#include <QVector>
#include <QPair>

#include <QHash>
#include <QSet>
#include <QMutex>

#include <DataTransferObjects/types.h>

#include <QTcpSocket>



class InvalidPasswordError : public std::exception
{
public:
    virtual const char* what() const throw()
    {
        return "Invalid password error";
    }
};

class TcpConnectionError : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Tcp connection error.";
    }
};


QTcpSocket* make_qTcpSocket(int socket_descriptor);



template<class T>
class SharedSet
{
protected:
    QSet<T> _set;
    QMutex _lock;
public:
    bool add(const T& key);
    bool remove(const T& key);
    int size() const;
};


#endif // UTILITY_H
