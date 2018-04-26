#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QTcpServer>
#include <my_server.h>

namespace Ui {
class Server;
class QLabel;
class QPushButton;
class QTcpServer;
class QNetworkSession;
}

class Server : public QMainWindow
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    ~Server();

private:
    Ui::Server *ui;
    My_server *server;
    void add_to_log(QString text, QColor color);

signals:
    void message_from_gui(QString message, const QStringList &users);
    void file_from_gui(QFile *new_file, const QStringList &users);

public slots:
    void on_add_user_to_gui(QString name);
    void on_remove_user_from_gui(QString name);
    void on_message_to_gui(QString message, QString from, const QStringList &users);
    void on_file_to_gui(QFile *new_file, QString from, const QStringList &users);
    void on_add_log_to_gui(QString string, QColor color);

private slots:
    void on_send_clicked();
    void on_check_to_all_clicked();
    void on_connect_toggled(bool checked);
    void on_disconnect_toggled(bool checked);
    void on_send_file_clicked();
};

#endif // MAINWINDOW_H
