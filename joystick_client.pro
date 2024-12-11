QT       += core gui gamepad websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# this is for Xbox controller - model:1914

#Xbox Wireless Controller - INPUT
# LS, RS : clickable analog sticks x 2
# LT, RT : analog triggers x 2
# LB, RB : digital shoulder buttons x 2
# A, B, X, Y, MENU, VIEW, XBOX : digital action buttons x 7
# up, down, left, right :  digital directional buttons(d-pad)  x 4
# Wireless pairing button
# SHARE :  Digital share button in fourth revision

SOURCES += \
    joystick_client.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    joystick_client.h \
    mainwindow.h

# Socket.io
INCLUDEPATH += /usr/local/include/
LIBS += -L/usr/local/lib/
LIBS += -lsioclient \
        -lsioclient_tls

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
