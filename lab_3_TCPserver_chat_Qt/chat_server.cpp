#include "chat_server.h"

Chat_server::Chat_server(QObject *parent) : QTcpServer(parent)
{
}

void Chat_server::StartServer()
{
    if (!this->listen(QHostAddress::Any, 1234))
        qDebug() << "Could not start server";
    else
        qDebug() << "Listening...";
}

void Chat_server::incomingConnection(int socket_descriptor)
{
    qDebug() << socket_descriptor << "Connecting ...";
    Chat_thread *thread = new Chat_thread(socket_descriptor, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}
