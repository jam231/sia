#ifndef LOGINSERVER_H
#define LOGINSERVER_H

#include <memory>

#include <QObject>
#include <QRunnable>
#include <QTcpServer>
#include <QEventLoop>

#include "connections.h"

#include <../Utilities/logger.h>

/*
 * Established connections and then handles only register and login requests.
 *
 */
class LoginServer : public QObject, public QRunnable
{    
protected:

    QEventLoop _event_loop;
    std::shared_ptr<AbstractLoggerFactory> _loggerFactory;

    QHash<int, std::shared_ptr<Connection> > connections;
    std::unique_ptr<QTcpServer> _server;
    int _port;

    void setupTcpServer();

public:
    LoginServer(std::shared_ptr<AbstractLoggerFactory> logger_factory,
                const QHash<QString, QString> &config);
    ~LoginServer();

    void run();

public slots:
    void newConnection();
};
#endif // LOGINSERVER_H
