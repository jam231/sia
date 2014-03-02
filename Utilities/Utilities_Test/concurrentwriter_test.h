#ifndef CONCURRENTWRITER_TEST_H
#define CONCURRENTWRITER_TEST_H


#include <QString>
#include <QtTest>


#include "mockobjects.h"

#include <memory>


class ConcurrentWriterTest : public QObject
{
    std::unique_ptr<MockWriter> _mock_writer;

    static const qint32 SHORT_STRING_LENGTH = 5,
                        LONG_STRING_LENGTH  = 100,
            // How many strings data_load for a worker contain.
                        SMALL_DATA_SET_SIZE = 100,
                        LARGE_DATA_SET_SIZE = 10000;

    Q_OBJECT

    void generate_data_for_rapidWriter(qint32 entry_length, qint32 data_set_size,
                                       qint32 max_writers_count, qint32 max_worker_count,
                                       qint32 writer_count_step = 2, qint32 worker_count_step=2
                                       );

private Q_SLOTS:
    void init();

    void rapidWrite_data();
    void rapidWrite();



};
#endif // CONCURRENTWRITER_TEST_H
