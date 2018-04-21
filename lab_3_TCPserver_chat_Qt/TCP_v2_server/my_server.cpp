#include "my_server.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QThread"

class Dialog;

My_server::My_server(QWidget *new_widget, QObject *parent) :QTcpServer(parent)
{
    widget = new_widget;
}

bool My_server::do_start_server(QHostAddress address, qint16 port)
{
    if (!listen(address, port))
    {
        qDebug() << "Server not started at" << address << ":" << port;
        return false;
    }
    qDebug() << "Server started at" << address << ":" << port;
    return true;
}

void My_server::do_send_to_all_user_join(QString name)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out << (quint16)0 << My_client::com_user_join << name;

    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    for (int i = 0; i < clients.length(); ++i)
        if ((clients.at(i)->get_name() != name) && (clients.at(i)->get_autched()))
            clients.at(i)->socket->write(block);
}

void My_server::do_send_to_all_user_left(QString name)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << My_client::com_user_left << name;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    for (int i = 0; i < clients.length(); ++i)
        if (clients.at(i)->get_name() != name && clients.at(i)->get_autched())
            clients.at(i)->socket->write(block);
}

void My_server::do_send_to_all_message(QString message, QString from_username)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << My_client::com_message_to_all << from_username << message;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    for (int i = 0; i < clients.length(); ++i)
        if (clients.at(i)->get_autched())
            clients.at(i)->socket->write(block);
}

void My_server::do_send_to_all_server_message(QString message)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << My_client::com_public_server_message << message;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    for (int i = 0; i < clients.length(); ++i)
        if (clients.at(i)->get_autched())
            clients.at(i)->socket->write(block);
}

void My_server::do_send_server_message_to_users(QString message, const QStringList &users)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << My_client::com_private_server_message << message;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    for (int j = 0; j < clients.length(); ++j)
        if (users.contains(clients.at(j)->get_name()))
            clients.at(j)->socket->write(block);
}

void My_server::do_send_message_to_users(QString message, const QStringList &users, QString from_username)
{
    QByteArray block, blockToSender;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << My_client::com_message_to_users << from_username << message;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));

    QDataStream outToSender(&blockToSender, QIODevice::WriteOnly);
    outToSender << (quint16)0 << My_client::com_message_to_users << users.join(",") << message;
    outToSender.device()->seek(0);
    outToSender << (quint16)(blockToSender.size() - sizeof(quint16));
    for (int j = 0; j < clients.length(); ++j)
        if (users.contains(clients.at(j)->get_name()))
            clients.at(j)->socket->write(block);
        else if (clients.at(j)->get_name() == from_username)
            clients.at(j)->socket->write(blockToSender);
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
    for (int i = 0; i < clients.length(); ++i)
        if (clients.at(i)->get_name() == name)
            return true;
    return false;
}

void My_server::incomingConnection(qintptr handle)
{

    My_client *client = new My_client(handle, this, this);
    if (widget != 0)
    {
        connect(client, SIGNAL(addUserToGui(QString)), widget, SLOT(on_add_user_to_gui(QString)));
        connect(client, SIGNAL(removeUserFromGui(QString)), widget, SLOT(on_remove_user_from_gui(QString)));
        connect(client, SIGNAL(messageToGui(QString,QString,QStringList)), widget, SLOT(on_message_to_gui(QString,QString,QStringList)));
    }
    connect(client, SIGNAL(removeuser(My_client*)), this, SLOT(on_remove_user(My_client*)));
    clients.append(client);
}

void My_server::on_remove_user(My_client *client)
{
    clients.removeAt(clients.indexOf(client));
}

void My_server::on_message_from_gui(QString message, const QStringList &users)
{
    if (users.isEmpty())
        do_send_to_all_server_message(message);
    else
        do_send_server_message_to_users(message, users);
}
