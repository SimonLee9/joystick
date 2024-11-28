#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

//#include <QHostAddress>
#include <QDebug>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <iostream>



QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void readData_1();

    void handleError(QSerialPort::SerialPortError error);

    void bt_usb1_con();
    void bt_usb1_dis();

private:
    Ui::MainWindow *ui;

    QSerialPort* m_serialPort1 = nullptr;
};
#endif // MAINWINDOW_H
