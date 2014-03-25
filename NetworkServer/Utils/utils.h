#ifndef UTILITY_H
#define UTILITY_H

#include <exception>
#include <memory>
#include <mutex>

#include <QVector>
#include <QPair>

#include <QHash>
#include <QSet>

#include <DataTransferObjects/types.h>

#include <QTcpServer>
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



class TcpServer : public QTcpServer
{

protected:
    virtual void incomingConnection(qintptr handle)
    {
        auto new_connection = new QTcpSocket();
        new_connection->setSocketDescriptor(handle);
        new_connection->setSocketOption(QAbstractSocket::SocketOption::KeepAliveOption, 1);
        addPendingConnection(new_connection);
    }
};



template<class T>
class SharedSet
{
protected:
    QSet<T> _set;
    std::mutex _lock;
public:
    bool add(const T& key);
    bool remove(const T& key);
    bool contains(const T& key);
    int size() const;
};


template<class T>
bool SharedSet<T>::add(const T& key)
{
    _lock.lock();
    bool will_be_added = _set.contains(key);
    if(!will_be_added)
    {
        _set.insert(key);
    }
    _lock.unlock();
    return !will_be_added;
}

template<class T>
bool SharedSet<T>::remove(const T& key)
{
    _lock.lock();
    bool will_be_removed = _set.contains(key);
    if(!will_be_removed)
    {
        _set.insert(key);
    }
    _lock.unlock();
    return !will_be_removed;
}

template<class T>
bool SharedSet<T>::contains(const T &key)
{
    _lock.lock();
    bool result = _set.contains(key);
    _lock.unlock();
    return result;
}

template<class T>
int SharedSet<T>::size() const
{
    return _set.size();
}


#endif // UTILITY_H
