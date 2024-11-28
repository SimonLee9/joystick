#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_serialPort1 = new QSerialPort(this);
    connect(m_serialPort1, &QSerialPort::readyRead, this, &MainWindow::readData_1);
    connect(m_serialPort1, &QSerialPort::errorOccurred, this, &MainWindow::handleError);

    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts())
    {
        ui->cb_usb1->addItem(port.portName());
    }

    connect(ui->bt_usb1_con, SIGNAL(clicked()), this, SLOT(bt_usb1_con()));
    connect(ui->bt_usb1_dis, SIGNAL(clicked()), this, SLOT(bt_usb1_dis()));


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::bt_usb1_con()
{

    qDebug()<<"open port : "<<ui->cb_usb1->currentText();
    // 시리얼 설정 코드
    //    m_serialPort->setPortName("/dev/ttyUSB0"); // 포트 이름 지정
    m_serialPort1->setPortName(ui->cb_usb1->currentText()); // 포트 이름 지정
    m_serialPort1->setBaudRate(QSerialPort::Baud9600); // baud: 초당 신호(siganl) 요소의 수 , 예) 하나의 버드에 2bit 있다면 1Baud 동안 2bit 전송 됨
    m_serialPort1->setDataBits(QSerialPort::Data8); // dataBits

    //    m_serialPort->setBaudRate(QSerialPort::Baud9600); // baud: 초당 신호(siganl) 요소의 수 , 예) 하나의 버드에 2bit 있다면 1Baud 동안 2bit 전송 됨
    //    m_serialPort->setDataBits(QSerialPort::Data104); // dataBits
    m_serialPort1->setParity(QSerialPort::NoParity); // 정보 전달 과정에 오류가 생겼는지 검사하기 위한 것
    m_serialPort1->setStopBits(QSerialPort::OneStop); // 포트를 열기전에 set 또는 success 하면 return true로 반환된다.
    m_serialPort1->setFlowControl(QSerialPort::NoFlowControl); // 흐름제어
    if (m_serialPort1->open(QIODevice::ReadWrite)) { //시리얼 장치 연 후 작업들


        ui->bt_usb1_con->setEnabled(false);
        std::cout<<"open"<<std::endl;
        ui->bt_usb1_dis->setEnabled(true);
    }
    else {
        std::cout<<"ERROR"<<std::endl;
        //        QMessageBox::critical(this, tr("Error"), m_serialPort->errorString());
    }
}


void MainWindow::bt_usb1_dis()
{
    if (m_serialPort1->isOpen()) // 시리얼 포트 열렸으면
        m_serialPort1->close(); // 시리얼 닫아라
    ui->bt_usb1_con->setEnabled(true);
    ui->bt_usb1_dis->setEnabled(false);
}



void MainWindow::readData_1()
{

    QByteArray serialData = m_serialPort1->readAll();
    qDebug() << serialData;
    ui->te_usb1->setText(serialData);

}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        std::cout<<"Error"<<std::endl;
        bt_usb1_dis();
    }
}
