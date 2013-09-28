#ifndef IOMESSAGETEST_H
#define IOMESSAGETEST_H

#include "../NetworkProtocol/iomessage.h"

#include <QString>
#include <QtTest>

class IOMessageTest : public QObject
{
    Q_OBJECT

public:
    IOMessageTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

#endif // IOMESSAGETEST_H
