#include "chat_thread.h"

Chat_thread::Chat_thread(int ID, QObject *parent) : QThread(parent)
{
    this->socket_descriptor = ID;
}

void Chat_thread::run()
{
    //thread starts here
    qDebug() << socket_descriptor << "Starting thread";
    socket = new QTcpSocket();

    if (!socket->setSocketDescriptor(this->socket_descriptor)) {
        return;
    }

    connect(socket, SIGNAL(readyRead()), this, SLOT(readyRead()), Qt::DirectConnection);
    connect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()), Qt::DirectConnection);

    qDebug() << socket_descriptor << "Client connected";

    exec();
}

void Chat_thread::readyRead()
{
    QByteArray Data = socket->readAll();

    qDebug() << socket_descriptor << "Data in : " << Data;
    socket->write(Data);
}

void Chat_thread::disconnected()
{
    qDebug() << socket_descriptor << "Disconnect : ";

    socket->deleteLater();
    exit(0);

}
