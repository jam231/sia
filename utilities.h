#ifndef UTILITY_H
#define UTILITY_H

#include <exception>

#include <QVector>
#include <QPair>


class InvalidPasswordError : public std::exception
{
    virtual const char* what() const throw()
    {
        return "Invalid password error";
    }
};


template<typename L, typename R>
struct Either
{
    bool isLeft;
    L left;
    R right;
};

template<typename T>
struct Maybe
{
    Maybe() : isNothing(true), value(T()) {}
    Maybe(T val) : isNothing(false), value(val) {}
    bool isNothing;
    T value;
    operator bool() const
    {
        return !isNothing;
    }
};

#endif // UTILITY_H
