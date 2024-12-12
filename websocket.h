#ifndef WEBSOCKET_UI_H
#define WEBSOCKET_UI_H

#include <mainwindow.h>
//#include <QObject>
//#include <QWebSocket>
//#include <QWebSocketServer>
//#include <QJsonArray>
//#include <QJsonObject>
//#include <QJsonDocument>
//#include <QDebug>


#include <queue>



class websocket_ui : public QObject
{
    Q_OBJECT
public:
    explicit websocket_ui(QObject *parent = nullptr);

    // websocekt server for slamnav2
    QWebSocketServer *server;
    QWebSocket *client = NULL;

    QList<QWebSocket *> clients;

    std::atomic<bool> is_connected = {false};

    void open();


    //QString get_json(QJsonObject& json, QString key);

    ST_STATUS   STATUS;
    std::queue<QString> msg;

    QTimer msg_timer;

    QString last_goal_id;
    QString old_cur_node_name= "move";

    bool start = false;

signals:

    void move_cmd_check_state(int msg,QString last_goal_id);
    void start_flag(bool start);
    void robot_node_pose(QString msg);

public slots:

    void client_connected();
    void client_disconnected();
    //void recv_message(QString message);
    void send_message();


};

#endif // WEBSOCKET_UI_H
