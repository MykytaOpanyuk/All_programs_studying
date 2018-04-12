#ifndef CHAT_SERVER_H
#define CHAT_SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QDebug>
#include "chat_thread.h"

class Chat_server : public QTcpServer
{
public:
    explicit Chat_server(QObject *parenr = 0);
    void StartServer();

   protected:
    void incomingConnection(int socket_descriptor);
};

#endif // CHAT_SERVER_H
