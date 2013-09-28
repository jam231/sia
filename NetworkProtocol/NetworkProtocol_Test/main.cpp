#include "iomessagetest.h"

int main(int argc, char *argv[])
{
    bool result = 0;

    IOMessageTest iomsg_test;

    result |= QTest::qExec(&iomsg_test, argc, argv);

    return result;
}



