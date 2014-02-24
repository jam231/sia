#include "bufferedwriter_test.h"
//#include <QtGlobal>

#include <utility>

using namespace std;

/// TODO:
///     Implement stubs

void BufferedWriterTest::init()
{
    _mock_writer = unique_ptr<MockWriter>(new MockWriter());
    assert(_mock_writer->buffer.empty());
}

void BufferedWriterTest::write_data()
{
    QTest::addColumn<qint32>("buffer_size");
    QTest::addColumn<QStringList>("data_load");
    QTest::addColumn<qint32>("chunk_size");
    QTest::addColumn<qint32>("chunks_output_count");
    //QTest::addColumn<qint32>("left_in_buffer_count");

    QStringList data_load;
    data_load << "This" << " " << "is" << " " << "test" << " " << "data";

    // For small dataset
    for(int i = 1; i < 12; i++)
    {
        QTest::newRow(qPrintable(QString("Writer with buffer size = %1 - small data set.").arg(i)))
                // Write request to underlying buffer is issued when the there is new write request
                // but the buffer is full.
                << i << data_load << i << (data_load.size() - 1) / i;// << data_load.size() % i;
    }

    QStringList large_data_load;

    // Generate huge data_set
    for(int i = 0; i < 1024 * 4; i++)
    {
        large_data_load << QString('a', (i+1)) << " ";
    }

    int buffer_size = 1;
    QTest::newRow(qPrintable(QString("Writer with buffer size = %1 - large data set.").arg(buffer_size)))
            << buffer_size << large_data_load << buffer_size << (large_data_load.size() - 1) / buffer_size;

    // For large dataset
    for(int i = 1; i < 15; i++)
    {
        buffer_size = 1 << i;
        QTest::newRow(qPrintable(QString("Writer with buffer size = %1 - large data set.").arg(buffer_size)))
                << buffer_size << large_data_load << buffer_size << (large_data_load.size() - 1) / buffer_size;
                //<< large_data_load.size() % buffer_size;
        buffer_size += 1;
        QTest::newRow(qPrintable(QString("Writer with buffer size = %1 - large data set.").arg(buffer_size)))
                << buffer_size << large_data_load << buffer_size << (large_data_load.size() - 1) / buffer_size;
                //<< large_data_load.size() % buffer_size;
    }

}

void BufferedWriterTest::write()
{
    QFETCH(qint32, buffer_size);
    QFETCH(QStringList, data_load);
    QFETCH(qint32, chunk_size);
    QFETCH(qint32, chunks_output_count);
    //QFETCH(qint32, left_in_buffer_count);

    QStringList* mock_writers_buffer = &(_mock_writer->buffer);
    BufferedWriter writer(move(_mock_writer), buffer_size);

    foreach (const QString &message, data_load)
    {
        writer.write(message);
    }

    QVERIFY2(mock_writers_buffer->size() == chunks_output_count,
             "Incorrect number of messages passed to the next writer.");

    // [msg1, msg2, ..., msgn] => [msg1 + msg2 + ... msgi, msg(i+1) + ..., ...]
    QStringList output;
    int i = 0;
    for(;i < data_load.size(); i+= chunk_size)
    {

        if(i + chunk_size < data_load.size())
        {
            QStringList chunk;
            for(int j = 0; j < chunk_size; j++)
            {
             assert(i + j < data_load.size());
             chunk << data_load[i + j];
            }
            assert(chunk.size() == chunk_size);
            output << chunk.join("");
        }
    }
    QVERIFY2(mock_writers_buffer->size() == output.size(),
             qPrintable(QString("Is %1 should be %2").arg(chunks_output_count)
                        .arg(output.size())));
    //assert (chunks_output_count == output.size());
    /*
    QStringList left_in_buffer;
    for(; i < data_load.size(); i++)
    {
        left_in_buffer << data_load[i];
    }
    assert (left_in_buffer_count == left_in_buffer);
    */
    for(int i = 0; i < output.size(); i++)
    {
        QVERIFY2(mock_writers_buffer->at(i) == output[i],
                 qPrintable(QString("Incorrect message passed to the next writer."\
                                    "Should be %1 is %2").arg(mock_writers_buffer->at(i)).arg(output[i])));
    }

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
