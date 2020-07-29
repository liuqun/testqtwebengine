QT += core gui
QT += webenginewidgets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mywebengine
TEMPLATE = app
!gcc: QMAKE_CXXFLAGS+="/utf-8"
win32: RC_ICONS=logo.ico

SOURCES += \
    main.cpp\
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += mywebengine_zh_CN.ts

RESOURCES += \
    mywebengine.qrc
