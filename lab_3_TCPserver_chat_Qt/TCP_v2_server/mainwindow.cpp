#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "my_server.h"
#include <QMessageBox>
#include <QFileDialog>

Server::Server(QWidget *parent) :QMainWindow(parent), ui(new Ui::Server)
{
    ui->setupUi(this);

    server = new My_server(this, this);

    connect(this, SIGNAL(messageFromGui(QString,QStringList)), server, SLOT(on_message_from_gui(QString,QStringList)));
    connect(server, SIGNAL(addLogToGui(QString,QColor)), this, SLOT(on_add_log_to_gui(QString,QColor)));

    if (server->do_start_server(QHostAddress::LocalHost, 55655)) {
        ui->list_info->insertItem(0, QTime::currentTime().toString()+" server started at " + server->serverAddress().toString()
                                  + ":" + QString::number(server->serverPort()));
        ui->list_info->item(0)->setTextColor(Qt::green);
        //ui->disconnect->setChecked(true);
    }
    else {
        ui->list_info->insertItem(0, QTime::currentTime().toString() + " server not started at " + server->serverAddress().toString()
                                  + ":" + QString::number(server->serverPort()));
        ui->list_info->item(0)->setTextColor(Qt::red);
        ui->connect->setChecked(true);
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
        if (ui->users_list->item(i)->text() == name)
        {
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


void Server::on_connect_toggled(bool checked)
{
    if (checked) {
        QHostAddress address;

        if (!address.setAddress(ui->IP->text())) {
            add_to_log(" invalid address " + ui->IP->text(), Qt::red);
            return;
        }
        if (server->do_start_server(address, ui->Port->text().toShort())) {
            add_to_log("Server started at " + ui->IP->text()+ ":" + ui->IP->text(), Qt::green);
            //ui->connect->setEnabled(false);
            //ui->disconnect->setEnabled(true);
        }
        else {
            add_to_log("Server not started at " + ui->IP->text() + ":" + ui->Port->text(), Qt::red);
            add_to_log("Change Port of your server!", Qt::red);
            //ui->connect->setEnabled(true);
            //ui->disconnect->setEnabled(false);
            ui->connect->setChecked(true);
        }
    }
}

void Server::on_disconnect_toggled(bool checked)
{
    if (checked) {
        add_to_log(" server stopped at " + server->serverAddress().toString() + ":"
                   + QString::number(server->serverPort()), Qt::green);
        server->close();
        //ui->connect->setEnabled(true);
        //ui->disconnect->setEnabled(false);
    }

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
}
