#ifndef UTILITY_H
#define UTILITY_H

#include <exception>

#include <QVector>
#include <QPair>

#include <QHash>

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



#endif // UTILITY_H
