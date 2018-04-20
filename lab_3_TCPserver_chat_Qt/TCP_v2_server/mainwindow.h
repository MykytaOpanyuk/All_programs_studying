#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QTcpServer>

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
    void sessionOpened();
    void sendFortune();

private:
    Ui::Server *ui;
    QTcpServer *tcpServer;
    QStringList fortunes;
    QNetworkSession *networkSession;
};

#endif // MAINWINDOW_H
