#include "mainwindow.h"
//#include "ui_mainwindow.h"
#include <QGamepadManager>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    //, ui(new Ui::MainWindow)
    , m_gamepad(nullptr)
    , m_timer(new QTimer(this))
    , m_reconnectTimer(new QTimer(this))
    , m_reconnectTimer2(new QTimer(this))
    , server("jog_server", QWebSocketServer::NonSecureMode, this)
    //, is_connected(false)
{
    //ui->setupUi(this);

    // websocket server open
    //if(server.listen(QHostAddress::Any, 11338)) // mobile server port + 1
    if(server.listen(QHostAddress::Any, 13359))
    {
        connect(&server, SIGNAL(newConnection()), this, SLOT(client_connected()));
        printf("[JOG] listen\n");
        qDebug()<< "[JOG] New Connection";
    }

    //reconnectGamepad();


    // 2초 주기로 게임패드 연결 상태 체크
    connect(m_reconnectTimer, &QTimer::timeout, this, &MainWindow::checkGamepadConnection);
    m_reconnectTimer->start(2000); // 2초마다 연결 상태 점검

    // 2초 주기로 게임패드 연결 상태 체크
    connect(m_reconnectTimer2, &QTimer::timeout, this, &MainWindow::checkSlamnavConnection);
    m_reconnectTimer2->start(2000); // 2초마다 연결 상태 점검

    QGamepadManager *manager = QGamepadManager::instance();
    if (manager->connectedGamepads().isEmpty())
    {
        qWarning()<<"[JOG] joystick :No connected!";
    }
    else
    {
        qDebug() <<"[JOG] joystick CONNECTED";
        int gamepadIndex = manager->connectedGamepads().first();
        m_gamepad = new QGamepad(gamepadIndex, this);

        connect(m_gamepad, &QGamepad::buttonAChanged, this, [=](bool pressed)
        {
            if (pressed)
            {
                qDebug() << "[JOG] Button A Pressed, triggering bt_JogF click.";
                //bt_JogF->click();
            }
        });

        connect(m_gamepad, &QGamepad::buttonGuideChanged, this, &MainWindow::onGuideButtonPressed);


        // QTimer를 사용하여 주기적으로 현재 게임패드 상태를 출력
        connect(m_timer, &QTimer::timeout, this, &MainWindow::updateGamepadStatus);
        m_timer->start(100); // 100ms 주기
    }

}

MainWindow::~MainWindow()
{
    //delete ui;
}

void MainWindow::connectToSlamnav(const QUrl &url)
{
    qDebug() << "[JOG] Connecting to slamnav server." << url.toString();
    //m_client.open(url);
}

void MainWindow::client_connected()
{
    qDebug() << "[JOG] Connected to slamnav server";
    is_connected = true;
    //연결직후 필요한 초기 통신이 있다면 여기서 수행

    QWebSocket *socket = server.nextPendingConnection();
    connect(socket, &QWebSocket::disconnected, this, &MainWindow::client_disconnected);

    client = socket;
    //is_connected = true;

    printf("[JOG] client connected, ip:%s\n", socket->peerAddress().toString().toLocal8Bit().data());
}
void MainWindow::client_disconnected()
{
    qDebug() << "[JOG] Disconnected from slamnav server!";
    is_connected = false;
    // 재접속 로직을 구현하려면 여기에 추가 가능


    QWebSocket *socket = qobject_cast<QWebSocket*>(sender());
    disconnect(socket, &QWebSocket::disconnected, this, &MainWindow::client_disconnected);
    client->deleteLater();

    printf("[JOG] client disconnected, ip:%s\n", socket->peerAddress().toString().toLocal8Bit().data());

}

void MainWindow::client_reconnection()
{
    qDebug() << "[JOG] try to reconnection slamnav server!";

}

void MainWindow::updateGamepadStatus()
{
    if(m_gamepad || is_connected)
    {
        // 조이스틱 값 읽기
        double lx = m_gamepad->axisLeftX();   // 왼스틱 X축
        double ly = m_gamepad->axisLeftY();   // 왼스틱 Y축
        double rx = m_gamepad->axisRightX();  // 오른스틱 X축

        // 간단한 스케일링:
        // 예) 최대 속도를 0.5 m/s, 최대 회전속도를 0.5 rad/s로 가정
        double max_linear_speed = 0.5;
        double max_angular_speed = 0.5;

        double vx = -ly * max_linear_speed;  // 전후진
        double vy = lx * max_linear_speed;  // 좌우 이동
        double wz = rx * max_angular_speed; // 회전

        // 현재 시간(초 단위) * 1000해서 time 매개변수로 전달
        double t = get_time0() * 1000.0;

        //qDebug()<<"is connected"<<is_connected;

        //if(m_gamepad->buttonL1() && is_connected)
        //{
        //    send_jog_command(vx, vy, wz, t);
        //}

        if (is_connected)
        {
            // slamnav에 jog 명령 전송
            send_jog_command(vx, vy, wz, t);
        }
    }


    /*
    qDebug() << "--- Xbox Controller Status ---";
    // Stick
    qDebug() << "Left Stick X Axis:" << m_gamepad->axisLeftX();
    qDebug() << "Left Stick Y Axis:" << m_gamepad->axisLeftY();
    qDebug() << "Right Stick X Axis:" << m_gamepad->axisRightX();
    qDebug() << "Right Stick Y Axis:" << m_gamepad->axisRightY();

    // Face Buttons
    qDebug() << "Button A Pressed:" << m_gamepad->buttonA();
    qDebug() << "Button B Pressed:" << m_gamepad->buttonB();
    qDebug() << "Button X Pressed:" << m_gamepad->buttonX();
    qDebug() << "Button Y Pressed:" << m_gamepad->buttonY();

    // Bumper, trigger
    qDebug() << "Button LB Pressed:" << m_gamepad->buttonL1();
    qDebug() << "Button RB Pressed:" << m_gamepad->buttonR1();
    qDebug() << "Left Trigger Pressure:" << m_gamepad->buttonL2();
    qDebug() << "Right Trigger Pressure:" << m_gamepad->buttonR2();

    // D-Pad
    qDebug() << "DPad Up Pressed:" << m_gamepad->buttonUp();
    qDebug() << "DPad Down Pressed:" << m_gamepad->buttonDown();
    qDebug() << "DPad Left Pressed:" << m_gamepad->buttonLeft();
    qDebug() << "DPad Right Pressed:" << m_gamepad->buttonRight();

    // 추가 버튼들 (Start, Select, Guide, L3, R3)
    qDebug() << "Button Start Pressed:" << m_gamepad->buttonStart();   // menu
    qDebug() << "Button Select Pressed:" << m_gamepad->buttonSelect(); // share
    qDebug() << "Button Guide Pressed:" << m_gamepad->buttonGuide();   // xbox indicator

    qDebug() << "--------------------------------";
    */

}

void MainWindow::send_jog_command(double vx, double vy, double wz, double time_ms)
{

    QJsonObject obj;
    obj["type"] = "move";
    obj["command"] = "jog";
    obj["vx"] = QString::number(vx);
    obj["vy"] = QString::number(vy);
    obj["wz"] = QString::number(wz);
    obj["time"] = QString::number((long long)(get_time0()*1000), 10);


    //printf("[COMM_UI] move, jog, t: %.3f, vel: %.3f, %.3f, %.3f\n", time_ms, vx, vy, wz);

    QJsonDocument doc(obj);
    QString str(doc.toJson());

    if (str.size()!=0)
    {
        client->sendTextMessage(str);
    }
}

void MainWindow::checkGamepadConnection()
{
    QGamepadManager *manager = QGamepadManager::instance();
    bool connected = !manager->connectedGamepads().isEmpty();

    if (!connected)
    {
        // 연결되어 있지 않은 상태라면 재시도
        qDebug() << "[JOG] joystick No detected. Trying to reconnect...";
        //reconnectGamepad();
        //client_connected();
    }

    // 이미 연결되어 있다면 아무것도 하지 않음.
    // 연결되어 있지만 m_gamepad가 null인 상태라면 reconnectGamepad를 호출할 수도 있음.
    // 여기서는 m_gamepad가 null이면 재연결 시도 가능
    //if (connected && !m_gamepad)
    if (connected)
    {
        //qDebug() << "[JOG] joystick found but not initialized, reconnecting...";
        //reconnectGamepad();
    }
}

void MainWindow::checkSlamnavConnection()
{
    //qDebug() << "[JOG] check slamnavconnection";
}

//void MainWindow::reconnectGamepad()
//{
//    // 기존 m_gamepad 삭제
//    if (m_gamepad)
//    {
//        delete m_gamepad;
//        m_gamepad = nullptr;
//    }
//
//    QGamepadManager *manager = QGamepadManager::instance();
//    if (manager->connectedGamepads().isEmpty())
//    {
//        qWarning() << "[JOG] joystick No connected! Cannot reconnect.";
//        return;
//    }
//    int gamepadIndex = manager->connectedGamepads().first();
//    m_gamepad = new QGamepad(gamepadIndex, this);
//
//    qDebug() << "[JOG] joystick reconnected successfully!";
//}

void MainWindow::onGuideButtonPressed(bool pressed)
{
    if (pressed)
    {
        qDebug() << "[JOG] Guide button pressed. Attempting to reconnect joystick...";
        //reconnectGamepad();
    }
}
