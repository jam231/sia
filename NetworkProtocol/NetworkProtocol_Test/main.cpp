
#include "DataTransferObjects/bestorder_test.h"

/// TODO:
///
/// - tests for remaining DataTransferObjects (Order, LastTransaction)
/// - tests for responses
/// - tests for requests
int main(int argc, char *argv[])
{
    bool result = 0;

    BestOrderTest bestOrder_test;

    result |= QTest::qExec(&bestOrder_test, argc, argv);


    return result;
}



