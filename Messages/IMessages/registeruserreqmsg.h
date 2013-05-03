#ifndef REGISTERUSERREQMSG_H
#define REGISTERUSERREQMSG_H

#include <exception>

#include <QString>

#include "imessage.h"


class InvalidPasswordError : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Invalid password error";
    }
};


class RegisterUserReqMsg : public IMessage
{
    QString m_password;
public:
    RegisterUserReqMsg(QIODevice* msg);

    MessageType type() const;
    QString getPassword() const;
};

#endif // REGISTERUSERREQMSG_H
