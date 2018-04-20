#ifndef MY_CLIENT_H
#define MY_CLIENT_H

#include <QObject>
#include <QDebug>
#include <QTcpSocket>
#include <QThreadPool>
#include <QtGui>
#include <QRegExp>
//#include "my_server.h"
class My_server;

class My_client : public QObject
{
    friend class my_server;
    Q_OBJECT

public:
    static const QString const_name_unknown;
    static const quint8 com_autch_request = 1;
    static const quint8 com_users_online = 2;
    static const quint8 com_user_join = 3;
    static const quint8 com_user_left = 4;
    static const quint8 com_message_to_all = 5;
    static const quint8 com_message_to_users = 6;
    static const quint8 com_public_server_message = 7;
    static const quint8 com_private_server_message = 8;
    static const quint8 com_autch_success = 9;
    static const quint8 com_error_name_invalid = 201;
    static const quint8 com_error_name_used = 202;

    explicit My_client(int new_decriptor, My_server *current_server, QObject *parent = 0);
    ~My_client();
    void set_name(QString new_name)
    {
        name = new_name;
    }
    QString get_name() const
    {
        return name;
    }
    bool get_autched() const
    {
        return is_autched;
    }
    void do_send_command(quint8 command) const;
    void do_send_users_online() const;

signals:
    void add_user_to_gui(QString name);
    void remove_user_from_gui(QString name);
    void remove_user(My_client *client);
    void message_to_gui(QString message, QString from, const QStringList &users);

private slots:
    void on_connect() {};
    void on_disconnect();
    void on_ready_read();
    void on_error(QAbstractSocket::SocketError socketError) const;

private:
    QTcpSocket *socket;
    My_server *server;
    quint16 block_size;
    QString name;
    bool is_autched;

};
#endif // MY_CLIENT_H
