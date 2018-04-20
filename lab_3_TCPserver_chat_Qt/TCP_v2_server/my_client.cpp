#include "my_client.h"

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
        server->do_send_to_all_user_left(name);
        emit remove_user(this);
    }
    deleteLater();
}

void My_client::onError(QAbstractSocket::SocketError socketError) const
{
    QWidget w;
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(&w, "Error", "The host was not found");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(&w, "Error", "The connection was refused by the peer.");
        break;
    default:
        QMessageBox::information(&w, "Error", "The following error occurred: " + socket->errorString());
    }
}

void My_client::onReadyRead()
{
    QDataStream in(socket);
    if (block_Size == 0) {
        if (socket->bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> block_size;
        qDebug() << "block_size now " << block_size;
    }

    if (socket->bytesAvailable() < block_size)
        return;
    else
        block_size = 0;

    quint8 command;
    in >> command;
    qDebug() << "Received command " << command;

    if (!is_autched && command != com_autch_Request)
        return;

    switch(command) {
        case com_Autch_request: {
            QString name;
            in >> name;

            if (server->is_name_valid(name)) {
                do_Send_command(com_err_name_invalid);
                return;
            }
            if (server->is_name_used(name)) {
                do_send_command(com_err_name_used);
                return;
            }
            name = name;
            is_autched = true;
            do_send_users_online();
            emit add_user_to_gui(name);
            server->do_send_to_all_user_join(name);
        }
        break;
        case com_message_to_all: {
            QString message;
            in >> message;
            server->do_send_to_all_message(message, name);
            emit message_to_gui(message, name, QStringList());
        }
        break;
        case com_message_to_users: {
            QString users_in;
            in >> users_in;
            QString message;
            in >> message;
            QStringList users = users_in.split(",");
            server->do_send_message_to_users(message, users, name);
            emit message_to_gui(message, name, users);
        }
        break;
    }

    //for (long long i = 0; i < 4000000000; ++i){}
}

void My_client::do_send_command(quint8 comm) const
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << comm;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    socket->write(block);
    qDebug() << "Send to" << name << "command:" << comm;
}

void My_client::do_send_users_online() const
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    out << com_users_online;
    QStringList l = server->get_users_online();
    QString s;
    for (int i = 0; i < l.length(); i++)
        if (l.at(i) != name)
            s = s + l.at(i) + (QString)",";
    s.remove(s.length() - 1, 1);
    out << s;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    socket->write(block);
    qDebug() << "Send user list to" << name << ":" << s;
}
