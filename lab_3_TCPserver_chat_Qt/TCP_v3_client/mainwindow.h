#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QtNetwork>
#include <QTcpSocket>
#include "peermanager.h"
#include "connection.h"
#include "client.h"

namespace Ui {
class MainWindow;
}
class PeerManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    Ui::MainWindow *ui;
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //void displayError(QAbstractSocket::SocketError socketError);
public slots:
    void appendMessage(const QString &from, const QString &message);

private slots:
    void returnPressed();
    void newParticipant(const QString &nick);
    void participantLeft(const QString &nick);
    void showInformation();

private:
    Client client;
    QString myNickName;
    QTextTableFormat tableFormat;
};

#endif // MAINWINDOW_H
