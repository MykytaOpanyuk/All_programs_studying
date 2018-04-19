#include "client.h"
#include "ui_client.h"
#include <QHostInfo>
#include <QMessageBox>
#include <QTime>
#include <QTcpSocket>

Client::Client(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Client)
{
    ui->setupUi(this);

    name = "";
    new_socket = new QTcpSocket(this);
    connect(new_socket, SIGNAL(readyRead()), this, SLOT(onSokReadyRead()));
    connect(new_socket, SIGNAL(connected()), this, SLOT(onSokConnected()));
    connect(new_socket, SIGNAL(disconnected()), this, SLOT(onSokDisconnected()));
    connect(new_socket, SIGNAL(error(QAbstractSocket::SocketError)),this, SLOT(onSokDisplayError(QAbstractSocket::SocketError)));
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
        if (new_socket->bytesAvailable() < (int)sizeof(quint16))
            return;
        in >> block_size;
        qDebug() << "_blockSize now " << block_size;
    }
    if (new_socket->bytesAvailable() < block_size)
        return;
    else
        block_size = 0;
    quint8 command;
    in >> command;
    qDebug() << "Received command " << command;

    switch (command)
    {
        case MyClient::comAutchSuccess:
        {
            ui->send_message->setEnabled(true);
            add_to_log("Enter as " + name, Qt::green);
        }
        break;
        case MyClient::comUsersOnline:
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
        case MyClient::comPublicServerMessage:
        {
            QString message;
            in >> message;
            add_to_log("[PublicServerMessage]: " + message, Qt::red);
        }
        break;
        case MyClient::comMessageToAll:
        {
            QString user;
            in >> user;
            QString message;
            in >> message;
            add_to_log("["+user+"]: " + message);
        }
        break;

        case MyClient::comMessageToUsers:
        {
            QString user;
            in >> user;
            QString message;
            in >> message;
            add_to_log("[" + user + "](private): " + message, Qt::blue);
        }
        break;
        case MyClient::comPrivateServerMessage:
        {
            QString message;
            in >> message;
            add_to_serve("[PrivateServerMessage]: " + message, Qt::red);
        }
        break;
        case MyClient::comUserJoin:
        {
            QString name;
            in >> name;
            ui->users_list->addItem(name);
            add_to_log( name + " joined", Qt::green);
        }
        break;
        case MyClient::comUserLeft:
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
        case MyClient::comErrNameInvalid:
        {
            QMessageBox::information(this, "Error", "This name is invalid.");
            new_socket->disconnectFromHost();
        }
        break;
        case MyClient::comErrNameUsed:
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
    out << (quint16)0;
    out << (quint8)MyClient::comAutchReq;
    out << ui->username->text();
    name = ui->username->text();
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    new_socket->write(block);
}

void Client::on_socket_disconnected()
{
    ui->connect->setEnabled(true);
    ui->disconnect->setEnabled(false);
    ui->send_message->setEnabled(false);
    ui->users_list->clear();
    add_to_log("Disconnected from" + new_socket->peerAddress().toString() + ":" + QString::number(new_socket->peerPort()), Qt::green);
}

void Client::on_connect_clicked()
{
    new_socket->connectToHost(ui->adress->text(), ui->port->text());
}

void Client::on_disconnect_clicked()
{
    new_socket->disconnectFromHost();
}

void Client::on_send_to_all_clicked()
{
    if (ui->cbToAll->isChecked())
        ui->send_message->setText("Send To All");
    else
        ui->send_message->setText("Send To Selected");
}

void Client::on_send_clicked()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0;
    if (ui->cbToAll->isChecked())
        out << (quint8)MyClient::comMessageToAll;
    else {
        out << (quint8)MyClient::comMessageToUsers;
        QString s;
        foreach (QListWidgetItem *i, ui->lwUsers->selectedItems())
            s += i->text()+",";
        s.remove(s.length()-1, 1);
        out << s;
    }
    out << ui->message->document()->toPlainText();
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    new_socket->write(block);
    ui->message->clear();
}

void Client::add_to_log(QString text, QColor color)
{
    ui->chat->insertItem(0, QTime::currentTime().toString() + " " + text);
    ui->chat->item(0)->setTextColor(color);
}
