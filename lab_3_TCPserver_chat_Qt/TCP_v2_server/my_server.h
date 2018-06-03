#ifndef MY_SERVER_H
#define MY_SERVER_H

#include <QTcpServer>
#include <QDebug>
#include <QThreadPool>
#include "my_client.h"

class My_client;

class My_server : public QTcpServer
{
    Q_OBJECT

public:
    QThreadPool *pool;
    QList<My_client *> clients;

    explicit My_server(QWidget *new_widget = 0, QObject *parent = 0);
    bool do_start_server(QHostAddress address, qint16 port);

    QStringList get_users_online() const;
    bool is_name_valid(QString name) const;
    bool is_name_used(QString name) const;

signals:
    void add_log_to_gui(QString string, QColor color = Qt::black);
    void add_file_to_gui(QString file_name, QString& from, QStringList &users);

protected:
    void incomingConnection(qintptr handle);

private:
    QWidget *widget;

};

#endif // MY_SERVER_H
