#include "globalutilities_test.h"


int main(int argc, char *argv[])
{
    bool result = 0;

    GlobalUtilitiesTest globalUtilities_test;

    result |= QTest::qExec(&globalUtilities_test, argc, argv);

    return result;
}




