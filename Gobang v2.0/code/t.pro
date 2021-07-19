QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    choosescene.cpp \
    gamemodel.cpp \
    main.cpp \
    mainscene.cpp \
    mainsceneclient.cpp \
    mainsceneonline.cpp \
    mypushbotton.cpp \
    mainwindow.cpp \
    networkgameclient.cpp \
    networkgameserver.cpp \
    roomwindow.cpp \
    joinwindow.cpp

HEADERS += \
    choosescene.h \
    gamemodel.h \
    mainscene.h \
    mainsceneclient.h \
    mainsceneonline.h \
    mypushbotton.h \
    joinwindow.h \
    mainwindow.h \
    networkgameclient.h \
    networkgameserver.h \
    roomwindow.h

FORMS += \
    choosescene.ui \
    joinwindow.ui \
    mainwindow.ui \
    roomwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
