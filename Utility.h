#ifndef UTILITY_H
#define UTILITY_H

#include <QVector>
#include <QPair>
#include <QStringList>

/*
 *  It ought be lightweight, besides I really see
 *  not point in having additional methods or fields.
 */
template<typename L, typename R>
struct Either
{
    // Left means
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

QVector<QPair<QString, QString> > parseINI(const QStringList&);

#endif // UTILITY_H
