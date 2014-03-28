#ifndef EVENTSERVER_H
#define EVENTSERVER_H

#include <../Utilities/logger.h>
#include <DataStorage/datastorage.h>

#include <memory>


#include <QThread>

/*
 * Event server handles market events, e.g. session starting/stoping
 *
 */

class EventServer : public QThread
{
    std::shared_ptr<AbstractDataStorageFactory> _dataFactory;
    std::shared_ptr<AbstractLoggerFactory> _loggerFactory;

    std::unique_ptr<QTimer> _sessionOnTimer,
                            _sessionOffTimer;

    int _session_on_interval_ms,
        _session_off_interval_ms;

    Q_OBJECT
public:
    EventServer(std::shared_ptr<AbstractLoggerFactory>,
                std::shared_ptr<AbstractDataStorageFactory>,
                const QHash<QString, QString>& config);

    void run();
signals:
    void sessionActivated();
    void sessionDisactivated();

protected slots:
    void sessionOn();
    void sessionOff();

};

#endif // EVENTSERVER_H
