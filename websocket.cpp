#include "websocket.h"

websocket_ui::websocket_ui(QObject *parent)
    : QObject(parent)
{
    open();
}

void websocket_ui::open()
{    server = new QWebSocketServer("Robot WebSocket Server", QWebSocketServer::NonSecureMode, this); // 올바른 생성자 호출

     // websocket server open
     if(server->listen(QHostAddress::Any, 13359)) // mobile server port + 1
     {
         connect(server, SIGNAL(newConnection()), this, SLOT(client_connected()));
         printf("[UI] listen\n");
     }

     connect(&msg_timer,SIGNAL(timeout()),this,SLOT(send_message()));
      msg_timer.start(200);
}

// websocket
void websocket_ui::client_connected()
{
    qDebug() << "Connected to slamnav server";
    is_connected = true;
    start = true;

    //QWebSocket *socket = server.nextPendingConnection();
    QWebSocket *socket = server->nextPendingConnection();
    connect(socket, SIGNAL(textMessageReceived(QString)), this, SLOT(recv_message(QString)));
    connect(socket, SIGNAL(disconnected()), this, SLOT(client_disconnected()));

    client = socket;

    printf("[UI] client connected, ip:%s\n", socket->peerAddress().toString().toLocal8Bit().data());

    emit start_flag(start);

}

void websocket_ui::client_disconnected()
{
    is_connected = false;
    start = false;

    qDebug()<<"상대방의 접속이 끊겼습니다.";
    //    msg = false;
    //    emit msgSignal(msg);

    QWebSocket *socket = qobject_cast<QWebSocket *>(sender());
    // disconnect(socket, &QWebSocket::disconnected, this, &MainWindow::client_disconnected);
    // client->deleteLater();
    if(socket)
    {
        clients.removeAll(socket);
        socket->deleteLater();
    }

    //    printf("[UI] client disconnected, ip:%s\n", socket->peerAddress().toString().toLocal8Bit().data());
}


void websocket_ui::send_message()
{
    if(msg.size()!= 0)
    {
        QString new_msg = msg.front();
        qDebug()<<new_msg;
        client->sendTextMessage(new_msg);
        qDebug()<<"send to slamnav2";
        msg.pop();
    }

    // que overflow control
    if(msg.size() > 50)
    {
        msg.pop();
    }

}
//QString websocket_ui::get_json(QJsonObject& json, QString key)
//{
//    return json[key].toString();
//}
