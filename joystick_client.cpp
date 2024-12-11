// joystick.cpp
#include "joystick_client.h"

//#include <QGamepad>
//#include <QGamepadManager>
//#include <QObject>
#include <QDebug>
//#include <QTimer>
//#include <QtMath> // qFabs()

#include <QJsonDocument>
#include <QJsonObject>

JoystickClient::JoystickClient(QObject *parent) : QObject(parent), gamepad(nullptr)
{
    connect(&webSocket, &QWebSocket::connected, this, &JoystickClient::onConnected);
    connect(&webSocket, &QWebSocket::disconnected, this, &JoystickClient::onDisconnected);
    connect(&webSocket, QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error), this, &JoystickClient::onError);

    setupJoystick();
}

void JoystickClient::connectToServer(const QUrl &url)
{
    webSocket.open(url);
}

void JoystickClient::onConnected()
{
    qDebug() << "WebSocket connected!";
}

void JoystickClient::onDisconnected()
{
    qDebug() << "WebSocket disconnected!";
}

void JoystickClient::onError(QAbstractSocket::SocketError error)
{
    qWarning() << "WebSocket error:" << error;
}

void JoystickClient::setupJoystick()
{
    QGamepadManager *manager = QGamepadManager::instance();
    if (manager->connectedGamepads().isEmpty()) {
        qWarning() << "[JoystickClient] No gamepads connected!";
        return;
    }

    // 연결된 첫 번째 게임패드 사용
    int gamepadIndex = manager->connectedGamepads().first();
    gamepad = new QGamepad(gamepadIndex, this);

    // 게임패드 이벤트 연결
    connect(gamepad, &QGamepad::axisLeftXChanged, this, [this](double value) {
        onJoystickEvent(value, gamepad->axisLeftY(), 0.0);
    });

    connect(gamepad, &QGamepad::axisLeftYChanged, this, [this](double value) {
        onJoystickEvent(gamepad->axisLeftX(), value, 0.0);
    });

    connect(gamepad, &QGamepad::buttonR2Changed, this, [this](double value) {
        onJoystickEvent(gamepad->axisLeftX(), gamepad->axisLeftY(), value);
    });
}

void JoystickClient::onJoystickEvent(double vx, double vy, double wz)
{
    if (webSocket.state() == QAbstractSocket::ConnectedState) {
        QJsonObject json;
        json["vx"] = vx;
        json["vy"] = vy;
        json["wz"] = wz;

        QJsonDocument doc(json);
        QString jsonString = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
        webSocket.sendTextMessage(jsonString);
    } else {
        qWarning() << "[JoystickClient] WebSocket not connected!";
    }
}
