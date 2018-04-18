#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets>
#include <QtNetwork>
#include <QTcpSocket>
#include "peermanager.h"
#include "connection.h"
#include "server.h"


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    Ui::MainWindow *ui;

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void sendMessage(const QString &message);
    QString nickName() const;
    bool hasConnection(const QHostAddress &senderIp, int senderPort = -1) const;

    void newConnection(Connection *connection);
    void readyForUse();
    void displayError(QAbstractSocket::SocketError socketError);

signals:
    void newMessage(const QString &from, const QString &message);
    void newParticipant(const QString &nick);
    void participantLeft(const QString &nick);


private:
    QTcpSocket *tcpSocket;
    PeerManager *peerManager;
    Server server;
    QMultiHash<QHostAddress, Connection *> peers;
};


#endif // MAINWINDOW_H
