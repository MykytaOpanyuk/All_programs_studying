#include "my_server.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTextStream>

class Dialog;

My_server::My_server(QWidget *new_widget, QObject *parent) :QTcpServer(parent)
{
    widget = new_widget;
    pool = new QThreadPool(this);
    pool->setMaxThreadCount(8);
}

bool My_server::do_start_server(QHostAddress address, qint16 port)
{
    if (!listen(address, port)) {
        qDebug() << "Server didn`t start at" << address << ":" << port;
        return false;
    }
    qDebug() << "Server has started at" << address << ":" << port;
    return true;
}

QStringList My_server::get_users_online() const
{
    QStringList l;
    foreach (My_client * c, clients)
        if (c->get_autched())
            l << c->get_name();
    return l;
}

bool My_server::is_name_valid(QString name) const
{
    if (name.length() > 20 || name.length() < 5)
        return false;
    QRegExp r("[A-Za-z0-9_]+");
    return r.exactMatch(name);
}

bool My_server::is_name_used(QString name) const
{
    for (int i = 0; i < clients.length(); i++)
        if (clients.at(i)->get_name() == name)
            return true;
    return false;
}

void My_server::incomingConnection(qintptr handle)
{
    My_client *client = new My_client(handle, this, this);
    if (widget != 0) {
        connect(client, SIGNAL(add_user_to_gui(QString)), widget, SLOT(on_add_user_to_gui(QString)), Qt::QueuedConnection);
        connect(client, SIGNAL(remove_user_from_gui(QString)), widget, SLOT(on_remove_user_from_gui(QString)), Qt::QueuedConnection);
        connect(client, SIGNAL(message_to_gui(QString,QString,QStringList)), widget, SLOT(on_message_to_gui(QString,QString,QStringList)), Qt::QueuedConnection);
        connect(client, SIGNAL(add_file_to_gui(QString,QString,QStringList)), widget, SLOT(on_file_to_gui(QString,QString,QStringList)), Qt::QueuedConnection);
    }
    connect(client, SIGNAL(removeUser(My_client*)), this, SLOT(on_remove_user(My_client*)), Qt::QueuedConnection);
    clients.append(client);

    //my_task *Task = new my_task();
    //Task->setAutoDelete(true);
    //Task->get_client(this);
}

