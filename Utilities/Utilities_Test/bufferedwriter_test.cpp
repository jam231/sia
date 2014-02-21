#include "bufferedwriter_test.h"
//#include <QtGlobal>

void BufferedWriterTest::cleanup()
{
    _mock_writer.buffer.clear();
}

void BufferedWriterTest::write_data()
{
    QTest::addColumn<qint32>("buffer_size");
    QTest::addColumn<QStringList>("data_load");
    QTest::addColumn<qint32>("chunk_size");
    QTest::addColumn<qint32>("chunks_output");
    QTest::addColumn<qint32>("left_in_buffer");

    QStringList data_load;
    data_load << "This" << " " << "is" << " " << "test" << " " << "data";

    /// FINISH
    //QTest::newRow("Immediate writer (buffer size LEQ 1)") << 1 << data_load;
}

void BufferedWriterTest::write()
{
    /// STUB
}

void BufferedWriterTest::flush_data()
{
    /// STUB
}

void BufferedWriterTest::flush()
{
    /// STUB
}

void BufferedWriterTest::flushOnDestruction_data()
{
    /// STUB
}
void BufferedWriterTest::flushOnDestruction()
{
    /// STUB
}
