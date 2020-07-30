#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setWindowTitle("陆军83集团军装备分析系统");
    w.setWindowState(Qt::WindowMaximized);
    w.show();

    return a.exec();
}
