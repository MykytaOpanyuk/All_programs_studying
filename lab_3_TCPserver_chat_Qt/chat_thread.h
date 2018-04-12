#ifndef CHAT_THREAD_H
#define CHAT_THREAD_H

#include <QThread>
#include <QObject>
#include <QDebug>
#include <QTcpSocket>

class Chat_thread : public QThread
{
public:
    Chat_thread(int ID, QObject *parent);
    void run();

signals:
    void error(QTcpSocket::SocketError socket_error);

private:
    QTcpSocket *socket;
    int socket_descriptor;
public slots:
    void readyRead();
    void disconnected();
};

#endif // CHAT_THREAD_H
