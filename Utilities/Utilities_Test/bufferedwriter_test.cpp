#include "bufferedwriter_test.h"

#include <writer.h>

#include <utility>


using namespace std;

/// TODO:
///     Refactor

void BufferedWriterTest::init()
{
    _mock_writer = unique_ptr<MockWriter>(new MockWriter());
    assert(_mock_writer->buffer->empty());
}

void BufferedWriterTest::write_data()
{
    QTest::addColumn<qint32>("buffer_size");
    QTest::addColumn<QStringList>("data_load");
    QTest::addColumn<qint32>("chunk_size");
    QTest::addColumn<qint32>("chunks_output_count");

    QStringList data_load;
    data_load << "This" << " " << "is" << " " << "test" << " " << "data";

    // For small dataset
    for(int i = 1; i < 12; i++)
    {
        QTest::newRow(qPrintable(QString("Writer with buffer size = %1. Entries written: %2.")
                                 .arg(i).arg(data_load.size())))
                // Write request to underlying buffer is issued when the there is new write request
                // but the buffer is full.
                << i << data_load << i << (data_load.size() - 1) / i;
    }

    data_load.clear();

    // Generate huge data_set
    for(int i = 0; i < 1024 * 4; i++)
    {
        data_load << QString(i + 1, 'a') << " ";
    }

    int buffer_size = 1;
    QTest::newRow(qPrintable(QString("Writer with buffer size = %1. Entries written: %2.")
                             .arg(buffer_size).arg(data_load.size())))
            << buffer_size << data_load << buffer_size << (data_load.size() - 1) / buffer_size;

    // For large dataset
    for(int i = 1; i < 15; i+=2)
    {
        buffer_size = 1 << i;
        QTest::newRow(qPrintable(QString("Writer with buffer size = %1. Entries written: %2.")
                                 .arg(buffer_size).arg(data_load.size())))
                << buffer_size << data_load << buffer_size << (data_load.size() - 1) / buffer_size;
        buffer_size += 1;
        QTest::newRow(qPrintable(QString("Writer with buffer size = %1. Entries written: %2.")
                                 .arg(buffer_size).arg(data_load.size())))
                << buffer_size << data_load << buffer_size << (data_load.size() - 1) / buffer_size;
    }

}

void BufferedWriterTest::write()
{
    QFETCH(qint32, buffer_size);
    QFETCH(QStringList, data_load);
    QFETCH(qint32, chunk_size);
    QFETCH(qint32, chunks_output_count);

    std::shared_ptr<QStringList> mock_writers_buffer(_mock_writer->buffer);
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
    assert(mock_writers_buffer->size() == output.size());

    for(int i = 0; i < output.size(); i++)
    {
        QVERIFY2(mock_writers_buffer->at(i) == output[i],
                 qPrintable(QString("Incorrect message passed to the next writer."\
                                    "Should be %1 is %2")
                            .arg(mock_writers_buffer->at(i)).arg(output[i])));
    }

}

void BufferedWriterTest::flush_data()
{
    QTest::addColumn<qint32>("buffer_size");
    QTest::addColumn<QStringList>("data_load");
    QTest::addColumn<qint32>("chunk_size");
    QTest::addColumn<qint32>("chunks_output_count");
    QTest::addColumn<qint32>("chunks_left_in_buffer_count");

    QStringList data_load;
    data_load << "This" << " " << "is" << " " << "test" << " " << "data";


    // For small dataset
    for(int buffer_size = 1; buffer_size < 12; buffer_size++)
    {

        int left_in_buffer = data_load.size() - ((data_load.size() - 1) / buffer_size) * buffer_size;
        QTest::newRow(qPrintable(QString("Writer with buffer size = %1. Entries written: %2.")
                                 .arg(buffer_size).arg(data_load.size())))
                // Write request to underlying buffer is issued when the there is new write request
                // but the buffer is full.
                << buffer_size << data_load << buffer_size
                << (data_load.size() - 1) / buffer_size << left_in_buffer;
    }

    data_load.clear();

    // Generate huge data_set
    for(int i = 0; i < 1024 * 4; i++)
    {
        data_load << QString(i+1, 'a') << " ";
    }

    int buffer_size = 1;
    int left_in_buffer = data_load.size() - ((data_load.size() - 1) / buffer_size) * buffer_size;

    QTest::newRow(qPrintable(QString("Writer with buffer size = %1. Entries written: %2.")
                             .arg(buffer_size).arg(data_load.size())))
            << buffer_size << data_load << buffer_size
            << (data_load.size() - 1) / buffer_size << left_in_buffer;

    // For large dataset
    for(int i = 1; i < 15; i++)
    {
        buffer_size = 1 << i;
        left_in_buffer = data_load.size() - ((data_load.size() - 1) / buffer_size) * buffer_size;

        QTest::newRow(qPrintable(QString("Writer with buffer size = %1. Entries written: %2.")
                                 .arg(buffer_size).arg(data_load.size())))
                << buffer_size << data_load << buffer_size << (data_load.size() - 1) / buffer_size
                << left_in_buffer;


        buffer_size += 1;
        left_in_buffer = data_load.size() - ((data_load.size() - 1) / buffer_size) * buffer_size;

        QTest::newRow(qPrintable(QString("Writer with buffer size = %1. Entries written: %2.")
                                 .arg(buffer_size).arg(data_load.size())))
                << buffer_size << data_load << buffer_size
                << (data_load.size() - 1) / buffer_size << left_in_buffer;
    }
}

void BufferedWriterTest::flush()
{
    QFETCH(qint32, buffer_size);
    QFETCH(QStringList, data_load);
    QFETCH(qint32, chunk_size);
    QFETCH(qint32, chunks_output_count);
    QFETCH(qint32, chunks_left_in_buffer_count);

    std::shared_ptr<QStringList> mock_writers_buffer(_mock_writer->buffer);
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
    assert(mock_writers_buffer->size() == output.size());


    QStringList left_in_buffer;
    for(int i = data_load.size() - chunks_left_in_buffer_count; i < data_load.size(); i++)
    {
        left_in_buffer << data_load[i];
    }
    assert (chunks_left_in_buffer_count == left_in_buffer.size());

    mock_writers_buffer->clear();
    // Every pending message should be flushed into mock_writers_buffer.
    writer.flush();

    QString flushed = left_in_buffer.join("");
    assert(mock_writers_buffer->size() > 0);
    QVERIFY2(mock_writers_buffer->first() == flushed,
             qPrintable(QString("After flushing - incorrect message passed to the next writer."\
                                "Should be %1 is %2").arg(mock_writers_buffer->first()).arg(flushed)));


}

void BufferedWriterTest::flushOnDestruction_data()
{
    flush_data();

}
void BufferedWriterTest::flushOnDestruction()
{
    QFETCH(qint32, buffer_size);
    QFETCH(QStringList, data_load);
    QFETCH(qint32, chunk_size);
    QFETCH(qint32, chunks_output_count);
    QFETCH(qint32, chunks_left_in_buffer_count);

    std::shared_ptr<QStringList> mock_writers_buffer(_mock_writer->buffer);
    QStringList left_in_buffer;
    // Destruction at the end of scope.

    {
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
        assert(mock_writers_buffer->size() == output.size());

        for(int i = data_load.size() - chunks_left_in_buffer_count; i < data_load.size(); i++)
        {
            left_in_buffer << data_load[i];
        }
        assert (chunks_left_in_buffer_count == left_in_buffer.size());
        // Make space for flushed data.
        mock_writers_buffer->clear();
        // This function + 1 MockWriter instance inside BufferedWriter.
        assert(mock_writers_buffer.use_count() == 2);
    }


    QString flushed = left_in_buffer.join("");

    assert(mock_writers_buffer->size() > 0);

    QVERIFY2(mock_writers_buffer->first() == flushed,
             qPrintable(QString("After flushing - incorrect message passed to the next writer."\
                                "Should be %1 is %2").arg(mock_writers_buffer->first()).arg(flushed)));

    QVERIFY2(mock_writers_buffer.use_count() == 1,
             qPrintable(QString("Buffer has too many owners(%1). It may imply that BufferedWriter wasn't destroyed properly.")
                        .arg(mock_writers_buffer.use_count())));
}
