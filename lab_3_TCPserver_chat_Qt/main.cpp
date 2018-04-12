#include <QCoreApplication>
#include "chat_server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Chat_server server;
    server.StartServer();
    return a.exec();
}
