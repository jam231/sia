#include "globalutilities_test.h"
#include "bufferedwriter_test.h"

int main(int argc, char *argv[])
{
    bool result = 0;

    GlobalUtilitiesTest globalUtilities_test;
    BufferedWriterTest  bufferedWriter_test;

    result |= QTest::qExec(&globalUtilities_test, argc, argv);
    result |= QTest::qExec(&bufferedWriter_test, argc, argv);

    return result;
}




