#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWebEngineView>
#include <QLineEdit>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    const char *url = "http://127.0.0.1/";
    ui->setupUi(this);

    view = new QWebEngineView(this);
    view->load(QUrl(url));
    setCentralWidget(view);
    resize(1024, 680);

    progress = 0;

    // 关联信号和槽
    connect(view, &QWebEngineView::loadProgress, this, &MainWindow::setProgress);
    connect(view, &QWebEngineView::titleChanged, this, &MainWindow::adjustTitle);
    connect(view, &QWebEngineView::loadFinished, this, &MainWindow::finishLoading);

    locationEdit = new QLineEdit(this);
    locationEdit->setText(url);
    connect(locationEdit, &QLineEdit::returnPressed, this, &MainWindow::changeLocation);
    connect(view, &QWebEngineView::urlChanged, this, &MainWindow::updateLocationByQWebEngine);

    // 向工具栏添加动作和部件
    ui->mainToolBar->addAction(view->pageAction(QWebEnginePage::Back));
    ui->mainToolBar->addAction(view->pageAction(QWebEnginePage::Forward));
    ui->mainToolBar->addAction(view->pageAction(QWebEnginePage::Reload));
    ui->mainToolBar->addAction(view->pageAction(QWebEnginePage::Stop));
    ui->mainToolBar->addWidget(locationEdit);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 更新URL地址字符串
void MainWindow::updateLocationByQWebEngine()
{
    QUrl url = view->url();
    locationEdit->setText(url.toString());
}

// 地址栏按下回车处理槽
void MainWindow::changeLocation()
{
    QUrl url = QUrl(locationEdit->text());
    view->load(url);
    view->setFocus();
}

void MainWindow::setProgress(int p)
{
    progress = p;
    adjustTitle();
}

// 调整标题槽
void MainWindow::adjustTitle()
{
    if (progress <= 0) {
        return;
    }
    if (progress >= 100) {
        setWindowTitle(view->title());
    } else {
        setWindowTitle(QString("%1 (%2%)").arg(view->title()).arg(progress));
    }
}

// 加载完成处理槽
void MainWindow::finishLoading(bool finished)
{
    if (finished) {
        progress = 100;
        setWindowTitle(view->title());
    } else {
        setWindowTitle("web page loading error!");
    }
}
