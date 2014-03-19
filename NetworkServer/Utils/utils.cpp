#include "utils.h"


QTcpSocket* make_qTcpSocket(int socket_descriptor)
{
    auto qsocket = new QTcpSocket();
    qsocket->setSocketDescriptor(socket_descriptor);

    //from http://stackoverflow.com/questions/10445122/qtcpsocket-state-always-connected-even-unplugging-ethernet-wire

    //int enableKeepAlive = 1;
    //int fd = qsocket->socketDescriptor();
    //setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (char*)&enableKeepAlive, sizeof(enableKeepAlive));

    //int maxIdle = 10; /* seconds */
    //setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, (char*)&maxIdle, sizeof(maxIdle));

    //int count = 3;  // send up to 3 keepalive packets out, then disconnect if no response
    //setsockopt(fd, SOL_TCP, TCP_KEEPCNT, (char*)&count, sizeof(count));

    //int interval = 2;   // send a keepalive packet out every 2 seconds (after the 5 second idle period)
    //setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, (char*)&interval, sizeof(interval));

    return qsocket;
}
