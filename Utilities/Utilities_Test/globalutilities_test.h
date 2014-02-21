#ifndef GLOBALUTILITIESTEST_H
#define GLOBALUTILITIESTEST_H

#include <utilities.h>

#include <QString>
#include <QtTest>


class GlobalUtilitiesTest : public QObject
{
        Q_OBJECT
public:
    GlobalUtilitiesTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
};

#endif // GLOBALUTILITIESTEST_H
