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

    MainWindow w;
    w.show();

    return a.exec();
}



