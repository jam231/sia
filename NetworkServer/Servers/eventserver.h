#ifndef EVENTSERVER_H
#define EVENTSERVER_H

#include <QObject>
#include <QRunnable>
#include <QEventLoop>

#include <../Utilities/logger.h>
#include <DataStorage/datastorage.h>

#include <memory>

/*
 * Event server handles market events, e.g. session starting/stoping
 *
 *
 *
 *
 *
 */

/// STUB
class EventServer : public QObject, public QRunnable
{
    std::shared_ptr<AbstractDataStorageFactory> _dataFactory;
    std::shared_ptr<AbstractLoggerFactory> _loggerFactory;


    Q_OBJECT
public:
    explicit EventServer(std::shared_ptr<AbstractDataStorageFactory> dataFactory,
                         std::shared_ptr<AbstractLoggerFactory> loggerFactory);

    void run();

signals:

public slots:

};

#endif // EVENTSERVER_H
