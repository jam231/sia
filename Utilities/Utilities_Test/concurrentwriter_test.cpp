#include "concurrentwriter_test.h"

#include <writer.h>

#include <utility>
#include "writer.h"
#include <algorithm>
#include <assert.h>

#include <QList>
#include <QTime>
#include <QtConcurrent/QtConcurrent>
#include <QFutureSynchronizer>


using namespace std;


void ConcurrentWriterTest::init()
{
    _mock_writer = unique_ptr<MockWriter>(new MockWriter());
    assert(_mock_writer->buffer->empty());
}


void ConcurrentWriterTest::generate_data_for_rapidWriter(qint32 entry_length, qint32 data_set_size,
                                                         qint32 max_writers_count, qint32 max_worker_count,
                                                         qint32 writer_count_step, qint32 worker_count_step)
{
    for(int writers_count  = 1; writers_count  < max_writers_count; writers_count += writer_count_step)
    {
        QList<QStringList> writers_data_loads;
        for(int writer_no = 0; writer_no < writers_count ; writer_no++)
        {
            QStringList data_load;

            for(int k = 0; k < data_set_size; k++)
            {
                // Format "<sequence of one letter><space><number>"
                data_load << (QString(entry_length, static_cast<QChar>('A' + writer_no)) +
                              " " + QString::number(k));
            }
            writers_data_loads << data_load;
        }

        assert(writers_data_loads.size() == writers_count);

        for(int workers_count= 1; workers_count < std::min(writers_count, max_worker_count);
            workers_count += worker_count_step)
        {
            QTest::newRow(qPrintable(QString("Rapid write with %1 writers and %2 worker threads. Each writer writes %3 "\
                                             "entries each ~%4 characters long (overall ~%5 characters written).")
                                     .arg(writers_count).arg(workers_count).arg(data_set_size)
                                     .arg(entry_length).arg(data_set_size*entry_length*writers_count)))

                    << workers_count << writers_count << writers_data_loads;
        }
    }
}

void ConcurrentWriterTest::rapidWrite_data()
{

    QTest::addColumn<qint32>("worker_threads_count");
    QTest::addColumn<qint32>("writers_count");
    QTest::addColumn<QList<QStringList> >("writers_work_loads");



    // For small dataset
    generate_data_for_rapidWriter(SHORT_STRING_LENGTH, SMALL_DATA_SET_SIZE, 10, 3, 2, 1);
    // For large dataset
    generate_data_for_rapidWriter(LONG_STRING_LENGTH, LARGE_DATA_SET_SIZE, 10, 5, 2, 2);
}


void ConcurrentWriterTest::rapidWrite()
{
    QFETCH(qint32, worker_threads_count);
    QFETCH(qint32, writers_count);
    QFETCH(QList<QStringList>, writers_work_loads);


    int old_maxThreadCount = QThreadPool::globalInstance()->maxThreadCount();
    QThreadPool::globalInstance()->setMaxThreadCount(worker_threads_count);

    std::shared_ptr<QStringList> mock_writers_buffer(_mock_writer->buffer);
    std::shared_ptr<AbstractWriter> writer = shared_ptr<AbstractWriter>(
                new ThreadSafeWriter(move(_mock_writer)));
    /*
    std::shared_ptr<AbstractWriter> writer = shared_ptr<AbstractWriter>(
            new ThreadSafeWriter(async_writer));
    */
    QFutureSynchronizer<void> future_synchronizer;

    // Write
    assert(writers_count == writers_work_loads.size());
    for(int writer_index = 0; writer_index < writers_count; writer_index++)
    {
        future_synchronizer.addFuture(QtConcurrent::run([&](const QStringList& load)
                                        {
                                            for(int i = 0; i < load.size(); i++)
                                            {
                                                writer->write(load[i]);
                                            }
                                        }, writers_work_loads[writer_index]));
    }

    future_synchronizer.waitForFinished();

    QThreadPool::globalInstance()->setMaxThreadCount(old_maxThreadCount);

    // It's stable so if [(a,a,..., 5), ..., (a,a, ..., 3)] in mock_writers_buffer
    // then after sorting by relative order of that two elemenets will be preserved (=> failure ;-) )
    std::stable_sort(mock_writers_buffer->begin(), mock_writers_buffer->end(),
                     [] (QString s1, QString s2) {return s1[0] < s2[0];});
    QStringList serial_write;
    foreach(const QStringList& str, writers_work_loads)
    {
        serial_write += str;
    }

    QString should_be_string = serial_write.join("");
    QString result           = mock_writers_buffer->join("");
    QVERIFY2(result.size() == should_be_string.size(),
             qPrintable(QString("Some entries has been lost. Result string length is %1 should be %2")
                        .arg(result.size()).arg(should_be_string.size())));
    for(int i = 0; i < result.size(); i++)
    {
        QVERIFY2(result[i] == should_be_string[i],
                 qPrintable(QString("Write order has been violated. Mismatch on position %1."\
                                    " Is %2 should be %3.\n %4 \n \t VS \n %5 \n")
                            .arg(QString::number(i))
                            .arg(result)
                            .arg(should_be_string)
                            .arg(result)
                            .arg(should_be_string)));
    }
}

