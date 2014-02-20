#ifndef BESTORDERMSG_TEST_H
#define BESTORDERMSG_TEST_H

#include "../NetworkProtocol/Responses/showbestordermsg.h"

class ShowBestOrderTest : public QObject
{
    Q_OBJECT

public:
    ShowBestOrderTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testCase1();
    void testCase1_data();
};

#endif // BESTORDERMSG_TEST_H
