#include "client.h"
#include "ui_client.h"
#include <QHostInfo>
#include <QMessageBox>
#include <QTime>
#include <QFileDialog>
#include <QTcpSocket>
#include <QtGui>

Client::Client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);

    name = "";
    new_socket = new QTcpSocket(this);
    connect(new_socket, SIGNAL(readyRead()), this, SLOT(on_socket_ready_read()));
    connect(new_socket, SIGNAL(connected()), this, SLOT(on_socket_connected()));
    connect(new_socket, SIGNAL(disconnected()), this, SLOT(on_socket_disconnected()));
    connect(new_socket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(on_socket_display_error(QAbstractSocket::SocketError)));
}

Client::~Client()
{
    delete ui;
}

void Client::on_socket_display_error(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, "Error", "The host was not found");
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, "Error", "The connection was refused by the peer.");
        break;
    default:
        QMessageBox::information(this, "Error", "The following error occurred: " + new_socket->errorString());
    }
}

void Client::on_socket_ready_read()
{
    QDataStream in(new_socket);
    if (block_size == 0) {
        if (new_socket->bytesAvailable() < (qint64)sizeof(quint64))
            return;
        in >> block_size;
        qDebug() << "_blockSize now " << block_size;
    }
    if (new_socket->bytesAvailable() < (qint64)block_size)
        return;
    else
        block_size = 0;
    quint8 command;
    in >> command;
    qDebug() << "Received command " << command;

    switch (command)
    {
        case My_client::com_autch_success:
        {
            ui->send_message->setEnabled(true);
            add_to_log("Enter as " + name, Qt::green);
        }
        break;
        case My_client::com_users_online:
        {
            add_to_log("Received user list " + name, Qt::green);
            ui->send_message->setEnabled(true);
            QString users;
            in >> users;
            if (users == "")
                return;
            QStringList l =  users.split(",");
            ui->users_list->addItems(l);
        }
        break;
        case My_client::com_public_server_message:
        {
            QString message;
            in >> message;
            add_to_log("[PublicServerMessage]: " + message, Qt::red);
        }
        break;
        case My_client::com_message_to_all:
        {
            QString user;
            in >> user;
            QString message;
            in >> message;
            add_to_log("[" + user + "]: " + message);
        }
        break;

        case My_client::com_message_to_users:
        {
            QString user;
            in >> user;
            QString message;
            in >> message;
            add_to_log("[" + user + "](private): " + message, Qt::blue);
        }
        break;
        case My_client::com_file_to_all:
        {
            QString user, old_file_name;
            QByteArray file_byte_array;
            char *file_data;
            quint64 file_size;

            in >> user;
            in >> old_file_name;
            in >> file_size;
            file_data = new char[file_size];
            in.readRawData(file_data, file_size);
            file_byte_array.append(file_data + 4, file_size);
            delete[] file_data;

            QMessageBox::information(this, tr("New file"), old_file_name);
            QString file_name = QFileDialog::getSaveFileName(this, tr("Open file"), "home//", "All files (*.*)");
            if (file_name.isNull()) {
                break;
                return;
            }
            QFile file(file_name);
            file.open(QIODevice::WriteOnly);
            file.write(file_byte_array);
            file.close();
            add_to_log("[" + user + "](file): " + file_name);
        }
        break;

        case My_client::com_file_to_users:
        {
            QString user, old_file_name;
            QByteArray file_byte_array;
            char *file_data;
            quint64 file_size;

            in >> user;
            in >> old_file_name;
            in >> file_size;
            file_data = new char[file_size];
            in.readRawData(file_data, file_size);
            file_byte_array.append(file_data + 4, file_size);
            delete[] file_data;

            QMessageBox::information(this, tr("New file"), old_file_name);
            QString file_name = QFileDialog::getSaveFileName(this, tr("Save file"), "home//", old_file_name);
            if (file_name.isNull()) {
                break;
                return;
            }
            QFile file(file_name);
            file.open(QIODevice::WriteOnly);
            file.write(file_byte_array);
            file.close();

            add_to_log("[" + user + "](file): " + file_name, Qt::blue);
        }
        break;
        case My_client::com_private_server_message:
        {
            QString message;
            in >> message;
            add_to_log("[PrivateServerMessage]: " + message, Qt::red);
        }
        break;
        case My_client::com_user_join:
        {
            QString name;
            in >> name;
            ui->users_list->addItem(name);
            add_to_log( name + " joined", Qt::green);
        }
        break;
        case My_client::com_user_left:
        {
            QString name;
            in >> name;
            for (int i = 0; i < ui->users_list->count(); ++i)
                if (ui->users_list->item(i)->text() == name)
                {
                    ui->users_list->takeItem(i);
                    add_to_log(name + " left", Qt::green);
                    break;
                }
        }
        break;
        case My_client::com_error_name_invalid:
        {
            QMessageBox::information(this, "Error", "This name is invalid.");
            new_socket->disconnectFromHost();
        }
        break;
        case My_client::com_error_name_used:
        {
            QMessageBox::information(this, "Error", "This name is already used.");
            new_socket->disconnectFromHost();
        }
        break;
    }
}

void Client::on_socket_connected()
{
    ui->connect->setEnabled(false);
    ui->disconnect->setEnabled(true);
    block_size = 0;
    add_to_log("Connected to" + new_socket->peerAddress().toString() + ":" + QString::number(new_socket->peerPort()),Qt::green);

    //try autch
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint64)0;
    out << (quint8)My_client::com_autch_request;
    out << ui->username->text();
    name = ui->username->text();
    out.device()->seek(0);
    out << (quint64)(block.size() - sizeof(quint64));
    new_socket->write(block);
}

void Client::on_socket_disconnected()
{
    ui->connect->setEnabled(true);
    ui->disconnect->setEnabled(false);
    ui->send_message->setEnabled(false);
    ui->users_list->clear();
    add_to_log("Disconnected from " + new_socket->peerAddress().toString() + ":" + QString::number(new_socket->peerPort()), Qt::green);
}

void Client::on_connect_clicked()
{
    new_socket->connectToHost(ui->adress->text(), ui->port->text().toUShort());
    ui->connect->setEnabled(false);
}

void Client::on_disconnect_clicked()
{
    new_socket->disconnectFromHost();
    ui->connect->setEnabled(true);
}

void Client::on_send_to_all_clicked()
{
    if (ui->check_to_all->isChecked())
        ui->send_message->setText("Send To All");
    else
        ui->send_message->setText("Send To Selected");
}

void Client::on_send_message_clicked()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint64)0;
    if (ui->check_to_all->isChecked())
        out << (quint8)My_client::com_message_to_all;
    else {
        out << (quint8)My_client::com_message_to_users;
        QString s;
        foreach (QListWidgetItem *i, ui->users_list->selectedItems())
            s = s + i->text() + ",";
        s.remove(s.length() - 1, 1);
        out << s;
    }
    out << ui->message->document()->toPlainText();
    out.device()->seek(0);
    out << (quint64)(block.size() - sizeof(quint64));
    new_socket->write(block);
    ui->message->clear();
}

void Client::add_to_log(QString text, QColor color)
{
    ui->chat->insertItem(0, QTime::currentTime().toString() + " " + text);
    ui->chat->item(0)->setTextColor(color);
}

void Client::on_Send_file_clicked()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint64)0;

    QString file_name = QFileDialog::getOpenFileName(this, tr("Open file"), "home//", "All files (*.*)");
    if (file_name.isNull()) {
        QMessageBox::information(this, tr("Null File"), "No such file or directory.");
        return;
    }
    else
        QMessageBox::information(this, tr("File name"), file_name);
    QFile new_file(file_name);
    new_file.open(QIODevice::ReadOnly);

    if (ui->check_to_all->isChecked())
        out << (quint8)My_client::com_file_to_all;
    else {
        out << (quint8)My_client::com_file_to_users;
        QString s;
        foreach (QListWidgetItem *i, ui->users_list->selectedItems())
            s = s + i->text() + ",";
        s.remove(s.length() - 1, 1);
        out << s;
    }
    QByteArray file_data = new_file.readAll();
    out << new_file.fileName();
    out << (quint64)file_data.size();
    out << file_data;
    new_file.close();
    out.device()->seek(0);
    out << (quint64)(block.size() - sizeof(quint64));
    new_socket->write(block);
}
