#ifndef GLOBALUTILITIESTEST_H
#define GLOBALUTILITIESTEST_H

#include <utilities.h>

#include <QString>
#include <QtTest>
#include <vector>

class GlobalUtilitiesTest : public QObject
{
        Q_OBJECT

private Q_SLOTS:
    void defaultGlobalLogger();

    void setLoggerWithNull();

    void setLogger_data();
    void setLogger();

};

#endif // GLOBALUTILITIESTEST_H
