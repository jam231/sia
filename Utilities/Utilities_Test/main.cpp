#include "globalutilities_test.h"
#include "bufferedwriter_test.h"
#include "concurrentwriter_test.h"
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

    GlobalUtilitiesTest globalUtilities_test;
    BufferedWriterTest  bufferedWriter_test;
    ConcurrentWriterTest concurrentWriter_test;

    result |= QTest::qExec(&globalUtilities_test, argc, argv);
    result |= QTest::qExec(&bufferedWriter_test, argc, argv);
    result |= QTest::qExec(&concurrentWriter_test, argc, argv);

    return result;
}




