#include "mainwindow.h"

#include <QApplication>
#include <QGamepad>
#include <QGamepadManager>

#include <QObject>
#include <QDebug>
#include <QTimer>
#include <QPushButton>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // QGamepadManager를 사용해 연결된 게임패드 정보 확인
    QGamepadManager *manager = QGamepadManager::instance();
    if (manager->connectedGamepads().isEmpty()) {
        qWarning() << "No gamepads connected!";
        return -1; // 게임패드가 없으면 프로그램 종료
    }

    // 연결된 첫 번째 게임패드의 인덱스 사용
    int gamepadIndex = manager->connectedGamepads().first();
    QGamepad *gamepad = new QGamepad(gamepadIndex, &a);

    // QPushButton 인스턴스 생성 (UI 상의 bt_JogF 버튼)
    QPushButton bt_JogF;
    bt_JogF.setGeometry(50, 10, 41, 41);
    bt_JogF.setText("F");
    bt_JogF.setAutoRepeat(false);
    bt_JogF.setAutoRepeatDelay(100);

    // 게임패드의 A 버튼을 QPushButton 클릭 이벤트와 연결
    QObject::connect(gamepad, &QGamepad::buttonAChanged, [&](bool pressed) {
        if (pressed) {
            qDebug() << "Button A Pressed, triggering bt_JogF click.";
            bt_JogF.click();
        }
    });

    // QTimer를 사용하여 주기적으로 현재 게임패드 상태를 출력
    QTimer *timer = new QTimer(&a);
    QObject::connect(timer, &QTimer::timeout, [&]() {
        qDebug() << "--- Xbox Controller Status ---";

        // Stick
        qDebug() << "Left Stick X Axis:" << gamepad->axisLeftX();
        qDebug() << "Left Stick Y Axis:" << gamepad->axisLeftY();
        qDebug() << "Right Stick X Axis:" << gamepad->axisRightX();
        qDebug() << "Right Stick Y Axis:" << gamepad->axisRightY();

        // face
        qDebug() << "Button A Pressed:" << gamepad->buttonA();
        qDebug() << "Button B Pressed:" << gamepad->buttonB();
        qDebug() << "Button X Pressed:" << gamepad->buttonX();
        qDebug() << "Button Y Pressed:" << gamepad->buttonY();

        // Bumper, trigger
        qDebug() << "Button LB Pressed:" << gamepad->buttonL1();
        qDebug() << "Button RB Pressed:" << gamepad->buttonR1();
        qDebug() << "Left Trigger Pressure:" << gamepad->buttonL2();
        qDebug() << "Right Trigger Pressure:" << gamepad->buttonR2();

        // D-Pad
        qDebug() << "DPad Up Pressed:" << gamepad->buttonUp();
        qDebug() << "DPad Down Pressed:" << gamepad->buttonDown();
        qDebug() << "DPad Left Pressed:" << gamepad->buttonLeft();
        qDebug() << "DPad Right Pressed:" << gamepad->buttonRight();

        // 추가 버튼들 (Start, Select, Guide, L3, R3)
        qDebug() << "Button Start Pressed:" << gamepad->buttonStart();      //menu
        qDebug() << "Button Select Pressed:" << gamepad->buttonSelect();    //share
        qDebug() << "Button Guide Pressed:" << gamepad->buttonGuide();      //xbox indicator

        qDebug() << "--------------------------------";
    });

    // 타이머 시작 (100ms 주기)
    timer->start(100);

    return a.exec();
}



