#include "message_test.h"

int main(int argc, char *argv[])
{
    bool result = 0;

    MessageTest message_test;

    result |= QTest::qExec(&message_test, argc, argv);

    return result;
}



