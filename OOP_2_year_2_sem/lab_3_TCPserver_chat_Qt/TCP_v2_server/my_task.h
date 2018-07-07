#ifndef MY_TASK_H
#define MY_TASK_H

#include <QRunnable>
#include <QDebug>
#include <QObject>
#include "my_client.h"
#include "my_server.h"

class My_client;
class My_server;

class my_task : public QObject, public QRunnable
{
    Q_OBJECT
public:
    My_client *client;
    qintptr socketDescriptor;
    virtual ~my_task() {};
    my_task();

    void get_client(My_client *new_client);
    void do_send_to_all_user_join(QString name);// when somebody join server
    void do_send_to_all_user_left(QString name, My_server *serv);// when somebody left server
    void do_send_to_all_message(QString message, QString from_username); // message from user to all
    void do_send_to_all_server_message(QString message); // server message to all
    void do_send_to_all_file(QFile *new_file, QString from_username); // file from user to all
    void do_send_to_all_server_file(QFile *new_file); // server file to all

    void do_send_server_message_to_users(QString message, const QStringList &users);//a server message to selected users (private)
    void do_send_message_to_users(QString message, const QStringList &users, QString from_username); //message from user to selected users (private)
    void do_send_server_file_to_users(QFile *new_file, const QStringList &users);//a server file to selected users (private)
    void do_send_file_to_users(QFile *new_file, const QStringList &users, QString from_username); //file from user to selected users (private)

public slots:
    void on_message_from_gui(QString message, const QStringList &users);
    void on_file_from_gui(QString file_name, const QStringList &users);
    void on_remove_user(My_client *client);


protected:
    void run();
};

#endif // MY_TASK_H
