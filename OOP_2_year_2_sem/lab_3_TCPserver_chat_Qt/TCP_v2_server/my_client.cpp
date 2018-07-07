#include "my_client.h"
#include "QMessageBox"
#include <QFileDialog>

const QString My_client::const_name_unknown = QString("Unknown");

My_client::My_client(int descriptor, My_server *current_server, QObject *parent) :QObject(parent)
{
    server = current_server;

    is_autched = false;
    name = const_name_unknown;

    block_size = 0;
    socket = new QTcpSocket(this);
    socket->setSocketDescriptor(descriptor);

    connect(socket, SIGNAL(connected()), this, SLOT(on_connect()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(on_disconnect()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(on_ready_read()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(on_error(QAbstractSocket::SocketError)));

    qDebug() << "Client connected" << descriptor;
}

My_client::~My_client()
{

}

void My_client::on_disconnect()
{
    qDebug() << "Client disconnected";
    if (is_autched) {
        emit remove_user_from_gui(name);
        emit removeUser(this);
        Task->do_send_to_all_user_left(name, server);
    }
    deleteLater();
}

void My_client::on_error(QAbstractSocket::SocketError socketError) const
{
    QWidget w;

    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(&w, "Error", "The host was not found.");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(&w, "Error", "The connection was refused by the peer.");
        break;
    default:
        QMessageBox::information(&w, "Error", "The following error occurred: " + socket->errorString());
    }
}

void My_client::on_ready_read()
{
    Task = new my_task();
    Task->setAutoDelete(true);
    Task->get_client(this);
    //connect(Task,SIGNAL(result()),this,SLOT(task_result()),Qt::QueuedConnection);
    this->server->pool->start(Task);
}

void My_client::task_result()
{

}

void My_client::do_send_command(quint8 comm) const
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint64)0;
    out << comm;
    out.device()->seek(0);
    out << (quint64)(block.size() - sizeof(quint64));
    socket->write(block);
    qDebug() << "Send to" << name << "command:" << comm;
}

void My_client::do_send_users_online() const
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint64)0;
    out << com_users_online;
    QStringList l = server->get_users_online();
    QString s;
    for (int i = 0; i < l.length(); i++)
        if (l.at(i) != name)
            s = s + l.at(i) + (QString)",";
    s.remove(s.length() - 1, 1);
    out << s;
    out.device()->seek(0);
    out << (quint64)(block.size() - sizeof(quint64));
    socket->write(block);
    qDebug() << "Send user list to" << name << ":" << s;
}
