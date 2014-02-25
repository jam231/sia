
#include "DataTransferObjects/bestorder_test.h"


int main(int argc, char *argv[])
{
    bool result = 0;

    BestOrderTest bestOrder_test;

    result |= QTest::qExec(&bestOrder_test, argc, argv);


    return result;
}



