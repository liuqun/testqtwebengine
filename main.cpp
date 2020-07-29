#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    QTranslator translator;

    const char *qmfile = "..\\..\\mywebengine\\mywebengine_zh_CN.qm";
    if(QFile::exists(qmfile)) {
        qWarning("failed-no file");
        return -1;
    }

    translator.load(qmfile);
    a.installTranslator(&translator);

    w.setWindowTitle("China No.22 Institude");
    w.show();

    return a.exec();
}
