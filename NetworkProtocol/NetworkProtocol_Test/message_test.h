#ifndef MESSAGE_TEST_H
#define MESSAGE_TEST_H

#include "../NetworkProtocol/message.h"

#include <QString>
#include <QtTest>

class MessageTest : public QObject
{
    Q_OBJECT

public:
    MessageTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

#endif // MESSAGE_TEST_H
