#ifndef MY_SERVER_H
#define MY_SERVER_H

#include <QTcpServer>
#include <QDebug>
#include "my_client.h"

class My_client;

class My_server : public QTcpServer
{
    Q_OBJECT

public:
    explicit My_server(QWidget *new_widget = 0, QObject *parent = 0);
    bool do_start_server(QHostAddress address, qint16 port);
    void do_send_to_all_user_join(QString name);// when somebody join server
    void do_send_to_all_user_left(QString name);// when somebody left server
    void do_send_to_all_message(QString message, QString from_username); // message from user to all
    void do_send_to_all_server_message(QString message); // server message to all
    void do_send_to_all_file(QFile *new_file, QString from_username); // file from user to all
    void do_send_to_all_server_file(QFile *new_file); // server file to all

    void do_send_server_message_to_users(QString message, const QStringList &users);//a server message to selected users (private)
    void do_send_message_to_users(QString message, const QStringList &users, QString from_username); //message from user to selected users (private)
    void do_send_server_file_to_users(QFile *new_file, const QStringList &users);//a server file to selected users (private)
    void do_send_file_to_users(QFile *new_file, const QStringList &users, QString from_username); //file from user to selected users (private)

    QStringList get_users_online() const;
    bool is_name_valid(QString name) const;
    bool is_name_used(QString name) const;

signals:
    void add_log_to_gui(QString string, QColor color = Qt::black);
    void add_file_to_gui(QString file_name, QString from, const QStringList &users);

public slots:
    void on_message_from_gui(QString message, const QStringList &users);
    void on_file_from_gui(QString file_name, const QStringList &users);
    void on_remove_user(My_client *client);

protected:
    void incomingConnection(qintptr handle);

private:
    QList<My_client *> clients;
    QWidget *widget;

};

#endif // MY_SERVER_H
