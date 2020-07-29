#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QTranslator translator;

    translator.load("../../mywebengine/mywebengine_zh_CN.qm");
    a.installTranslator(&translator);

    w.setWindowTitle("China No.22 Institude");
    w.show();

    return a.exec();
}
