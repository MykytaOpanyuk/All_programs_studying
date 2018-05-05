#include "my_client.h"
#include "my_server.h"
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
        server->do_send_to_all_user_left(name);
        emit removeUser(this);
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
    QDataStream in(socket);
    if (block_size == 0) {
        if (socket->bytesAvailable() < (qint64)sizeof(quint64))
            return;
        in >> block_size;
        qDebug() << "block_size now " << block_size;
    }

    if (socket->bytesAvailable() < (qint64)block_size)
        return;
    else
        block_size = 0;

    quint8 command;
    in >> command;
    qDebug() << "Received command " << command;

    if ((!is_autched) && (command != com_autch_request))
        return;

    switch(command) {
        case com_autch_request: {
            QString new_name;
            in >> new_name;

            if (!server->is_name_valid(new_name)) {
                do_send_command(com_error_name_invalid);
                return;
            }
            if (server->is_name_used(new_name)) {
                do_send_command(com_error_name_used);
                return;
            }
            name = new_name;
            is_autched = true;
            do_send_users_online();
            emit add_user_to_gui(new_name);
            server->do_send_to_all_user_join(name);
        }
        break;
        case com_file_to_all: {
            char *file_data;
            QString file_name;
            QByteArray file_byte_array;
            quint64 file_size;

            in >> file_name;
            in >> file_size;
            file_data = new char[file_size];
            in.readRawData(file_data, file_size);
            file_byte_array.append(file_data + 4, file_size);

            QString file_name_2;
            int i;
            for (i = (int)file_name.size(); i > 0; i--) {
                if (file_name[i] == '/')
                    break;
                file_name_2.push_front(file_name[i]);
            }

            QFile *new_file = new QFile(file_name_2);

            new_file->open(QIODevice::WriteOnly);
            new_file->write(file_byte_array);
            new_file->close();

            server->do_send_to_all_file(new_file, name);
            emit add_file_to_gui(new_file->fileName(), name, QStringList());
            delete[] file_data;
        }
        break;
        case com_file_to_users: {
            QString users_in;
            char *file_data;
            QString file_name;
            QByteArray file_byte_array;
            quint64 file_size;

            in >> users_in;
            in >> file_name;
            in >> file_size;
            file_data = new char[file_size];
            in.readRawData(file_data, file_size);
            file_byte_array.append(file_data + 4, file_size);

            QString file_name_2;
            int i;
            for (i = (int)file_name.size(); i > 0; i--) {
                if (file_name[i] == '/')
                    break;
                file_name_2.push_front(file_name[i]);
            }

            QFile *new_file = new QFile(file_name_2);

            new_file->open(QIODevice::WriteOnly);
            new_file->write(file_byte_array);
            new_file->close();

            QStringList users = users_in.split(",");
            server->do_send_file_to_users(new_file, users, name);
            emit add_file_to_gui(new_file->fileName(), name, users);
            delete[] file_data;
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
