#include "mainwindow.h"
#include "chatdialog.h"
#include "ui_mainwindow.h"
#include "client.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->comboBox->setEditable(true);
    // find out name of this machine
    QString name = QHostInfo::localHostName();
    if (!name.isEmpty()) {
        ui->comboBox->addItem(name);
        QString domain = QHostInfo::localDomainName();
        if (!domain.isEmpty())
            ui->comboBox->addItem(name + QChar('.') + domain);
    }

    ui->message->setFocusPolicy(Qt::StrongFocus);
    ui->textEdit->setFocusPolicy(Qt::NoFocus);
    ui->textEdit->setReadOnly(true);

    //connect(ui->message, SIGNAL(returnPressed()), this, SLOT(returnPressed()));
    connect(ui->message, SIGNAL(returnPressed()), this, SLOT(returnPressed()));
    connect(&client, SIGNAL(newMessage(QString,QString)),
            this, SLOT(appendMessage(QString,QString)));
    connect(&client, SIGNAL(newParticipant(QString)),
            this, SLOT(newParticipant(QString)));
    connect(&client, SIGNAL(participantLeft(QString)),
            this, SLOT(participantLeft(QString)));

    newParticipant(myNickName);
    tableFormat.setBorder(0);
    QTimer::singleShot(10 * 1000, this, SLOT(showInformation()));

}

void MainWindow::appendMessage(const QString &from, const QString &message)
{
    if (from.isEmpty() || message.isEmpty())
        return;

    QTextCursor cursor(ui->textEdit->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextTable *table = cursor.insertTable(1, 2, tableFormat);
    table->cellAt(0, 0).firstCursorPosition().insertText('<' + from + "> ");
    table->cellAt(0, 1).firstCursorPosition().insertText(message);
    QScrollBar *bar = ui->textEdit->verticalScrollBar();
    bar->setValue(bar->maximum());
}

void MainWindow::returnPressed()
{
    QString text = ui->message->text();
    if (text.isEmpty())
        return;

    if (text.startsWith(QChar('/'))) {
        QColor color = ui->textEdit->textColor();
        ui->textEdit->setTextColor(Qt::red);
        ui->textEdit->append(tr("! Unknown command: %1")
                         .arg(text.left(text.indexOf(' '))));
        ui->textEdit->setTextColor(color);
    } else {
        client.sendMessage(text);
        appendMessage(myNickName, text);
    }

    ui->message->clear();
}

void MainWindow::newParticipant(const QString &nick)
{
    if (nick.isEmpty())
        return;

    ui->textEdit->setTextColor(Qt::gray);
    ui->textEdit->append(tr("* %1 has joined").arg(nick));
    ui->listWidget->addItem(nick);
}

void MainWindow::participantLeft(const QString &nick)
{
    if (nick.isEmpty())
        return;

    QList<QListWidgetItem *> items = ui->listWidget->findItems(nick,
                                                           Qt::MatchExactly);
    if (items.isEmpty())
        return;

    delete items.at(0);
    QColor color = ui->textEdit->textColor();
    ui->textEdit->setTextColor(Qt::gray);
    ui->textEdit->append(tr("* %1 has left").arg(nick));
    ui->textEdit->setTextColor(color);
}

void MainWindow::showInformation()
{
    if (ui->listWidget->count() == 1) {
        QMessageBox::information(this, tr("Chat"),
                                 tr("Launch several instances of this "
                                    "program on your local network and "
                                    "start chatting!"));
    }
}


MainWindow::~MainWindow()
{
    delete ui;
}
/*
void MainWindow::displayError(QAbstractSocket::SocketError socketError)
{
    switch (socketError) {
    case QAbstractSocket::RemoteHostClosedError:
        break;
    case QAbstractSocket::HostNotFoundError:
        QMessageBox::information(this, tr("Chat"),
                                 tr("The host was not found. Please check the "
                                    "host name and port settings."));
        break;
    case QAbstractSocket::ConnectionRefusedError:
        QMessageBox::information(this, tr("Chat"),
                                 tr("The connection was refused by the peer. "
                                    "Make sure the fortune server is running, "
                                    "and check that the host name and port "
                                    "settings are correct."));
        break;
    default:
        QMessageBox::information(this, tr("Chat"),
                                 tr("The following error occurred: %1.")
                                 .arg(tcpSocket->errorString()));
    }

    ui->Send_file->setEnabled(true);
}
*/

