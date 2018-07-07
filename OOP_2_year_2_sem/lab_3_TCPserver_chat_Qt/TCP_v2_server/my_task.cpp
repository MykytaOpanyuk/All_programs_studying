#include "my_task.h"

my_task::my_task()
{
    qDebug() << "Task created";
}

void my_task::do_send_to_all_user_join(QString name)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    out << (quint64)0 << My_client::com_user_join << name;

    out.device()->seek(0);
    out << (quint64)(block.size() - sizeof(quint64));

    for (int i = 0; i < client->server->clients.length(); ++i)
        if ((client->server->clients.at(i)->get_name() != name) && (client->server->clients.at(i)->get_autched())) {
            client->server->clients.at(i)->socket->write(block);
            client->server->clients.at(i)->socket->flush();
            client->server->clients.at(i)->socket->waitForBytesWritten();
        }
}

void my_task::do_send_to_all_user_left(QString name, My_server *serv)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint64)0 << My_client::com_user_left << name;
    out.device()->seek(0);
    out << (quint64)(block.size() - sizeof(quint64));
    for (int i = 0; i < serv->clients.length(); i++)
        if (serv->clients.at(i)->get_name() != name && serv->clients.at(i)->get_autched()) {
            serv->clients.at(i)->socket->write(block);
            serv->clients.at(i)->socket->flush();
            serv->clients.at(i)->socket->waitForBytesWritten();
        }
}

void my_task::do_send_to_all_message(QString message, QString from_username)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint64)0 << My_client::com_message_to_all << from_username << message;
    out.device()->seek(0);
    out << (quint64)(block.size() - sizeof(quint64));
    for (int i = 0; i < client->server->clients.length(); ++i)
        if (client->server->clients.at(i)->get_autched()) {
            client->server->clients.at(i)->socket->write(block);
            client->server->clients.at(i)->socket->flush();
            client->server->clients.at(i)->socket->waitForBytesWritten();
        }
}

void my_task::do_send_to_all_server_message(QString message)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint64)0 << My_client::com_public_server_message << message;
    out.device()->seek(0);
    out << (quint64)(block.size() - sizeof(quint64));
    for (int i = 0; i < client->server->clients.length(); ++i)
        if (client->server->clients.at(i)->get_autched()) {
            client->server->clients.at(i)->socket->write(block);
            client->server->clients.at(i)->socket->flush();
            client->server->clients.at(i)->socket->waitForBytesWritten();
        }
}

void my_task::do_send_to_all_file(QFile *new_file, QString from_username)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    new_file->open(QIODevice::ReadOnly);
    QByteArray file_data = new_file->readAll();
    out << (quint64)0 << My_client::com_file_to_all << from_username << new_file->fileName();
    out << (quint64)file_data.size();
    out << file_data;
    new_file->close();
    delete new_file;
    out.device()->seek(0);
    out << (quint64)(block.size() - sizeof(quint64));

    for (int i = 0; i < client->server->clients.length(); i++)
        if (client->server->clients.at(i)->get_autched()) {
            client->server->clients.at(i)->socket->write(block);
            client->server->clients.at(i)->socket->flush();
            client->server->clients.at(i)->socket->waitForBytesWritten();
        }
}

void my_task::do_send_to_all_server_file(QFile *new_file)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    new_file->open(QIODevice::ReadOnly);
    QByteArray file_data = new_file->readAll();
    out << (quint64)0 << My_client::com_public_server_file << new_file->fileName();
    out << (quint64)file_data.size();
    out << file_data;
    new_file->close();
    delete new_file;
    out.device()->seek(0);
    out << (quint64)(block.size() - sizeof(quint64));
    for (int i = 0; i < client->server->clients.length(); ++i)
        if (client->server->clients.at(i)->get_autched()) {
            client->server->clients.at(i)->socket->write(block);
            client->server->clients.at(i)->socket->flush();
            client->server->clients.at(i)->socket->waitForBytesWritten();
        }
}
//private
void my_task::do_send_server_message_to_users(QString message, const QStringList &users)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint64)0 << My_client::com_private_server_message << message;
    out.device()->seek(0);
    out << (quint64)(block.size() - sizeof(quint64));
    for (int j = 0; j < client->server->clients.length(); ++j)
        if (users.contains(client->server->clients.at(j)->get_name())) {
            client->server->clients.at(j)->socket->write(block);
            client->server->clients.at(j)->socket->flush();
            client->server->clients.at(j)->socket->waitForBytesWritten();
        }
}

void my_task::do_send_server_file_to_users(QFile *new_file, const QStringList &users)
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);

    new_file->open(QIODevice::ReadOnly);
    QByteArray file_data = new_file->readAll();
    out << (quint64)0 << My_client::com_private_server_file << new_file->fileName();
    out << (quint64)file_data.size();
    out << file_data;
    new_file->close();
    out.device()->seek(0);
    out << (quint64)(block.size() - sizeof(quint64));
    delete new_file;
    for (int j = 0; j < client->server->clients.length(); ++j)
        if (users.contains(client->server->clients.at(j)->get_name())) {
            client->server->clients.at(j)->socket->write(block);
            client->server->clients.at(j)->socket->flush();
            client->server->clients.at(j)->socket->waitForBytesWritten();
        }
}

void my_task::do_send_message_to_users(QString message, const QStringList &users, QString from_username)
{
    QByteArray block, blockToSender;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint64)0 << My_client::com_message_to_users << from_username << message;
    out.device()->seek(0);
    out << (quint64)(block.size() - sizeof(quint64));

    QDataStream outToSender(&blockToSender, QIODevice::WriteOnly);
    outToSender << (quint64)0 << My_client::com_message_to_users << users.join(",") << message;
    outToSender.device()->seek(0);
    outToSender << (quint64)(blockToSender.size() - sizeof(quint64));
    for (int j = 0; j < client->server->clients.length(); ++j)
        if (users.contains(client->server->clients.at(j)->get_name())) {
            client->server->clients.at(j)->socket->write(block);
            client->server->clients.at(j)->socket->flush();
            client->server->clients.at(j)->socket->waitForBytesWritten();
        }
        else if (client->server->clients.at(j)->get_name() == from_username) {
            client->server->clients.at(j)->socket->write(blockToSender);
            client->server->clients.at(j)->socket->flush();
            client->server->clients.at(j)->socket->waitForBytesWritten();
        }
}

void my_task::do_send_file_to_users(QFile *new_file, const QStringList &users, QString from_username)
{
    QByteArray block, blockToSender;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint64)0 << My_client::com_file_to_users << from_username;
    new_file->open(QIODevice::ReadOnly);
    QByteArray file_data = new_file->readAll();
    out << new_file->fileName();
    out << (quint64)file_data.size();
    out << file_data;
    new_file->close();
    delete new_file;
    out.device()->seek(0);
    out << (quint64)(block.size() - sizeof(quint64));

    for (int j = 0; j < client->server->clients.length(); ++j)
        if (users.contains(client->server->clients.at(j)->get_name())) {
            client->server->clients.at(j)->socket->write(block);
            client->server->clients.at(j)->socket->flush();
            client->server->clients.at(j)->socket->waitForBytesWritten();
        }
}


void my_task::get_client(My_client *new_client)
{
    client = new_client;
}

void my_task::run()
{
    QDataStream in(client->socket);

        if (client->block_size == 0) {
            if (client->socket->bytesAvailable() < (qint64)sizeof(quint64))
                return;
            in >> client->block_size;
        }
        qDebug() << "block_size now " << client->block_size;

        if (client->socket->bytesAvailable() < (qint64)client->block_size)
            return;
        else
            client->block_size = 0;

        quint8 command;
        in >> command;
        qDebug() << "Received command " << command;

        if ((!client->is_autched) && (command != client->com_autch_request))
            return;

        switch(command) {
            case client->com_autch_request: {
                QString new_name;
                in >> new_name;

                if (!client->server->is_name_valid(new_name)) {
                    client->do_send_command(client->com_error_name_invalid);
                    return;
                }
                if (client->server->is_name_used(new_name)) {
                    client->do_send_command(client->com_error_name_used);
                    return;
                }
                client->name = new_name;
                client->is_autched = true;
                client->do_send_users_online();
                emit client->add_user_to_gui(new_name);
                do_send_to_all_user_join(client->name);
            }
            break;
            case client->com_file_to_all: {
                char *file_data;
                QString file_name;
                QByteArray file_byte_array;
                quint64 file_size;

                in >> file_name;
                in >> file_size;
                file_data = new char[file_size];
                in.readRawData(file_data, file_size);
                file_byte_array.append(file_data + 4, file_size);

                QString file_name_2;
                int i;
                for (i = (int)file_name.size(); i > 0; i--) {
                    if (file_name[i] == '/')
                        break;
                    file_name_2.push_front(file_name[i]);
                }

                QFile *new_file = new QFile(file_name_2);

                new_file->open(QIODevice::WriteOnly);
                new_file->write(file_byte_array);
                new_file->close();

                client->socket->readAll();

                do_send_to_all_file(new_file, client->name);

                delete[] file_data;
                emit client->add_file_to_gui(file_name_2, client->name, QStringList());
            }
            break;
            case client->com_file_to_users: {
                QString users_in;
                char *file_data;
                QString file_name;
                QByteArray file_byte_array;
                quint64 file_size;

                in >> users_in;
                in >> file_name;
                in >> file_size;

                file_data = new char[file_size];
                in.readRawData(file_data, file_size);
                file_byte_array.append(file_data + 4, file_size);

                QString file_name_2;
                int i;
                for (i = (int)file_name.size(); i > 0; i--) {
                    if (file_name[i] == '/')
                        break;
                    file_name_2.push_front(file_name[i]);
                }

                QFile *new_file = new QFile(file_name_2);

                new_file->open(QIODevice::WriteOnly);
                new_file->write(file_byte_array);
                new_file->close();

                client->socket->readAll();

                QStringList users = users_in.split(",");
                do_send_file_to_users(new_file, users, client->name);
                delete[] file_data;
                emit client->add_file_to_gui(file_name_2, client->name, users);
            }
            break;
            case client->com_message_to_all: {
                QString message;
                in >> message;
                do_send_to_all_message(message, client->name);
                emit client->message_to_gui(message, client->name, QStringList());
            }
            break;
            case client->com_message_to_users: {
                QString users_in;
                in >> users_in;
                QString message;
                in >> message;
                QStringList users = users_in.split(",");
                do_send_message_to_users(message, users, client->name);
                emit client->message_to_gui(message, client->name, users);
            }
            break;
        }
}

void my_task::on_remove_user(My_client *client)
{
    client->server->clients.removeAt(client->server->clients.indexOf(client));
}

void my_task::on_message_from_gui(QString message, const QStringList &users)
{
    if (users.isEmpty())
        do_send_to_all_server_message(message);
    else
        do_send_server_message_to_users(message, users);
}

void my_task::on_file_from_gui(QString file_name, const QStringList &users)
{
    QFile *new_file = new QFile(file_name);
    if (users.isEmpty())
        do_send_to_all_server_file(new_file); //deleting new_file in function
    else
        do_send_server_file_to_users(new_file, users); //deleting new_file in function

}
