// joystick.h
#ifndef JOYSTICK_CLIENT_H
#define JOYSTICK_CLIENT_H

#include <QObject>
//#include <QPushButton> // 지워도 되나?
#include <QGamepad>

#include <QWebSocket>

class JoystickClient : public QObject
{
    Q_OBJECT

public:
    explicit JoystickClient(QObject *parent = nullptr);
    //void initialize();
    void connectToServer(const QUrl &url);

private slots:
    void onConnected();
    void onDisconnected();
    void onError(QAbstractSocket::SocketError error);

    void onJoystickEvent(double vx, double vy, double wz);

//Q_SIGNALS:
//    void signal_buttonAChanged(bool pressed); // A 버튼 시그널
//    void signal_buttonBChanged(bool pressed); // B 버튼 시그널
//    void signal_buttonXChanged(bool pressed); // X 버튼 시그널
//    void signal_buttonYChanged(bool pressed); // Y 버튼 시그널
//
//    void signal_buttonASafety(bool pressed);
//    void signal_buttonBSafety(bool pressed);
//    void signal_buttonXSafety(bool pressed);
//    void signal_buttonYSafety(bool pressed);
//
//    void signal_buttonLBChanged(bool pressed);
//    void signal_buttonRBChanged(bool pressed);
//    //not use
//
//    void signal_joystickAxisChanged(double axisX, double axisY);




private:
    QWebSocket webSocket;

    QGamepad *gamepad;

    void setupJoystick();


    //QPushButton bt_JogF;
    //bool is_lb_pressed = false; // LB 버튼이 눌렸는지 여부를 추적
    //bool is_rb_pressed = false;
    //double joystick_gain = 0.1;
};

#endif // JOYSTICK_H
