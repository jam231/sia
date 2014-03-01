
#include "DataTransferObjects/bestorder_test.h"

/// TODO:
///
/// - tests for remaining DataTransferObjects (Order, LastTransaction)
/// - tests for responses
/// - tests for requests
///

/*
 *
 *  About methodology
 *      assert for is being used for asserting things about testing (not tested!) code -
 *      if assert doesn't hold then there is bug in test code.
 *
 *      QVERIFY2, QFAIL, etc. is being used for asserting some traits of tested code -
 *      if that fail then there is bug in tested code, and unit test proved its value.
 *
 */


int main(int argc, char *argv[])
{
    bool result = 0;

    BestOrderTest bestOrder_test;

    result |= QTest::qExec(&bestOrder_test, argc, argv);


    return result;
}



