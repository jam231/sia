
#include "DataTransferObjects/bestorder_test.h"
#include "DataTransferObjects/lasttransaction_test.h"
#include "DataTransferObjects/order_test.h"
/// TODO:
///
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

    BestOrderTest       bestOrder_test;
    LastTransactionTest lastTransaction_test;
    OrderTest           order_test;
    result |= QTest::qExec(&bestOrder_test, argc, argv);
    result |= QTest::qExec(&lastTransaction_test, argc, argv);
    result |= QTest::qExec(&order_test, argc, argv);


    return result;
}



