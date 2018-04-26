#ifndef CLIENT_H
#define CLIENT_H

#include <QDialog>
#include <QTcpSocket>
#include <QMainWindow>
#include <QHostAddress>
#include <QMessageBox>
#include "../TCP_v2_server/my_client.h"

namespace Ui {
class Client;
}

class QComboBox;
class QDialogButtonBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QTcpSocket;
class QNetworkSession;

class Client : public QMainWindow
{
    Q_OBJECT

public:
    explicit Client(QWidget *parent = 0);
    ~Client();

private slots:
    void on_socket_connected();
    void on_socket_disconnected();
    void on_socket_ready_read();
    void on_socket_display_error(QAbstractSocket::SocketError socketError);

    void on_connect_clicked();
    void on_disconnect_clicked();
    void on_send_to_all_clicked();
    void on_send_clicked();

    void on_send_message_clicked();

private:
    Ui::Client *ui;
    QTcpSocket *new_socket;
    quint16 block_size;
    QString name;
    void add_to_log(QString text, QColor color = Qt::black);

};

#endif // CLIENT_H
