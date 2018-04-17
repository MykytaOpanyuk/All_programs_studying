#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork>
#include <QTcpServer>

namespace Ui {
class MainWindow;
class QLabel;
class QPushButton;
class QTcpServer;
class QNetworkSession;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void sessionOpened();
    void sendFortune();

private:
    Ui::MainWindow *ui;
    QTcpServer *tcpServer;
    QStringList fortunes;
    QNetworkSession *networkSession;
};

#endif // MAINWINDOW_H
