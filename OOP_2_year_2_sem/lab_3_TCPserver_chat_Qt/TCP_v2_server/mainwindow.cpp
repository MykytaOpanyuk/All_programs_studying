#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "my_server.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>

Server::Server(QWidget *parent) :QMainWindow(parent), ui(new Ui::Server)
{
    ui->setupUi(this);

    server = new My_server(this, this);

    connect(this, SIGNAL(message_from_gui(QString,QStringList)), server, SLOT(on_message_from_gui(QString,QStringList)));
    connect(this, SIGNAL(file_from_gui(QString, QStringList)), server, SLOT(on_file_from_gui(QString,QStringList)));
    connect(server, SIGNAL(add_file_to_gui(QFile*,QString,const QStringList&)), server, SLOT(on_file_to_gui(
           QFile*, QString, const QStringList &)));
    connect(server, SIGNAL(add_log_to_gui(QString,QColor)), this, SLOT(on_add_log_to_gui(QString,QColor)));

    if (server->do_start_server(QHostAddress::LocalHost, 55655)) {
        ui->list_info->insertItem(0, QTime::currentTime().toString() + " server has started at " + server->serverAddress().toString()
                                  + ":" + QString::number(server->serverPort()));
        ui->list_info->item(0)->setTextColor(Qt::green);
    }
    else {
        ui->list_info->insertItem(0, QTime::currentTime().toString() + " server didn`t start at " + server->serverAddress().toString()
                                  + ":" + QString::number(server->serverPort()));
        ui->list_info->item(0)->setTextColor(Qt::red);
    }
}

Server::~Server()
{
    delete ui;
}

void Server::on_add_user_to_gui(QString name)
{
    ui->users_list->addItem(name);
    ui->list_info->insertItem(0, QTime::currentTime().toString() + " " + name + " joined to server");
    ui->list_info->item(0)->setTextColor(Qt::yellow);
}

void Server::on_remove_user_from_gui(QString name)
{
    for (int i = 0; i < ui->users_list->count(); i++)
        if (ui->users_list->item(i)->text() == name) {
            ui->users_list->takeItem(i);
            ui->list_info->insertItem(0, QTime::currentTime().toString() + " " + name + " left server");
            ui->list_info->item(0)->setTextColor(Qt::yellow);
            break;
        }
}

void Server::on_message_to_gui(QString message, QString from, const QStringList &users)
{
    if (users.isEmpty())
        ui->list_info->insertItem(0, QTime::currentTime().toString() + " message from " + from + ": " + message + " to all");
    else {
        ui->list_info->insertItem(0, QTime::currentTime().toString() + " message from " + from + ": " + message +
                                  " to " + users.join(","));
        ui->list_info->item(0)->setTextColor(Qt::blue);
    }
}

void Server::on_file_to_gui(QString file_name, QString from, const QStringList &users)
{
    if (users.isEmpty())
        ui->list_info->insertItem(0, QTime::currentTime().toString() + " file from " + from + ": " + file_name + " to all");
    else {
        ui->list_info->insertItem(0, QTime::currentTime().toString() + " file from " + from + ": " + file_name +
                                  " to " + users.join(","));
        ui->list_info->item(0)->setTextColor(Qt::blue);
    }
}

void Server::on_add_log_to_gui(QString string, QColor color)
{
    add_to_log(string, color);
}

void Server::on_send_clicked()
{
    if (ui->users_list->count() == 0) {
        QMessageBox::information(this, "", "Nobody connected to server!");
        return;
    }

    QStringList l;

    if (!ui->checkBox->isChecked())
        foreach (QListWidgetItem *i, ui->users_list->selectedItems())
            l << i->text();
    emit message_from_gui(ui->message->document()->toPlainText(), l);
    ui->message->clear();
    if (l.isEmpty())
        add_to_log("Sended public server message", Qt::black);
    else
        add_to_log("Sended private server message to "+l.join(","), Qt::black);
}

void Server::on_check_to_all_clicked()
{
    if (ui->checkBox->isChecked())
        ui->send->setText("Send To All");
    else
        ui->send->setText("Send To Selected");
}


void Server::on_connect_clicked()
{
    QHostAddress address;

    if (!address.setAddress(ui->IP->text())) {
        add_to_log(" invalid address " + ui->IP->text(), Qt::red);
        return;
    }

    if (server->do_start_server(address, server->serverPort())) {
        ui->port->setText(QString::number(server->serverPort()));
        add_to_log("Server has started at " + ui->IP->text()+ ":" + server->serverPort(), Qt::green);
    }
    else {
        add_to_log("Server didn't start at " + ui->IP->text() + ":" + server->serverPort(), Qt::red);
        add_to_log("Change Port of your server!", Qt::red);
    }
}

void Server::on_disconnect_clicked()
{
    add_to_log(" server has stopped at " + server->serverAddress().toString() + ":"
               + QString::number(server->serverPort()), Qt::green);
    server->close();
}

void Server::add_to_log(QString text, QColor color)
{
    ui->list_info->insertItem(0, QTime::currentTime().toString()+text);
    ui->list_info->item(0)->setTextColor(color);
}

void Server::on_send_file_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(this, tr("Open file"), "home//", "All files (*.*)");
    QMessageBox::information(this, tr("File name"), file_name);
    if (file_name.isNull())
        return;
    //QFile new_file(file_name);
    QStringList l;

    if (!ui->checkBox->isChecked())
        foreach (QListWidgetItem *i, ui->users_list->selectedItems())
            l << i->text();
    emit file_from_gui(file_name, l);
    if (l.isEmpty())
        add_to_log("Sended public server file", Qt::black);
    else
        add_to_log("Sended private server file to " + l.join(","), Qt::black);
}
