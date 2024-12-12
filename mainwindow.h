#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>

#include <QMainWindow>

#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QLabel>
#include <QLineEdit>


#include <QGamepad>
#include <QPushButton>

//websocket
#include <QWebSocket>
#include <QWebSocketServer>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include "common_data.h"



QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}

QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    // get system time
    double get_time0()
    {
        std::chrono::time_point<std::chrono::system_clock> t = std::chrono::system_clock::now();
        auto timestamp = std::chrono::duration_cast<std::chrono::nanoseconds>(t.time_since_epoch()).count();
        return (timestamp*1.0e-9);
    }

    void connectToSlamnav(const QUrl &url);

    void send_joystick_data(); //SLAMCMD_MOVE

    QString get_json(QJsonObject& json, QString key);

private slots:

    //websocket
    void client_connected();
    void client_disconnected();

    //void recv_message(QString message);

    void updateGamepadStatus();
    void checkGamepadConnection();
    void reconnectGamepad(); // Guide 버튼을 눌렀을 때 재연결 시도하는 함수
    void onGuideButtonPressed(bool pressed);
    void send_jog_command(double vx, double vy, double wz, double time_ms);


private:

    Ui::MainWindow *ui;
    //QSerialPort* m_serialPort1 = nullptr;
    QGamepad *m_gamepad;
    QTimer *m_timer;
    QTimer *m_reconnectTimer;

    //WebSocket 통신 객체
    QWebSocketServer server;
    QWebSocket *client = NULL;
    std::atomic<bool> is_connected = {false};

    //Websocket
    //QWebSocket m_client;
    //bool is_connected = false;
    //QUrl m_slamnavUrl;

};
#endif // MAINWINDOW_H
