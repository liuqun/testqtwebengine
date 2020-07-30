#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QWebEngineView>
#include <QWebEngineSettings>
#include <QLabel>
#include <QLineEdit>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    const char *url = "http://127.0.0.1/";
    ui->setupUi(this);

    view = new QWebEngineView(this);
    QWebEngineSettings *settings = view->settings();
    settings->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    view->load(QUrl(url));
    setCentralWidget(view);
    statusBar()->setHidden(true);

    progress = 0;

    // 关联信号和槽
    connect(view, &QWebEngineView::loadProgress, this, &MainWindow::setProgress);
    connect(view, &QWebEngineView::titleChanged, this, &MainWindow::adjustTitle);
    connect(view, &QWebEngineView::loadFinished, this, &MainWindow::finishLoading);
    connect(view, &QWebEngineView::iconUrlChanged,
            this, &MainWindow::handleIconUrlChanged);
    manager = new QNetworkAccessManager(this);

    webSiteIcon = new QIcon();
    webSiteIconLabel = new QLabel(this);
    webSiteIconLabel->setMargin(3);
    QImage image(QString(":/image/internet-web-browser.png"));
    webSiteIconLabel->setFixedSize(24, 24);
    webSiteIconLabel->setPixmap(
                QPixmap::fromImage(image.scaled(
                                       18, 18,
                                       Qt::KeepAspectRatio,
                                       Qt::SmoothTransformation)));

    locationEdit = new QLineEdit(this);
    locationEdit->setText(url);
    locationEdit->setAlignment(Qt::AlignmentFlag::AlignLeft);
    locationEdit->setDisabled(true);
    locationEdit->setStyleSheet("QLineEdit{border-width:0;border-style:outset}");
    connect(locationEdit, &QLineEdit::returnPressed, this, &MainWindow::changeLocation);
    connect(view, &QWebEngineView::urlChanged, this, &MainWindow::updateLocationByQWebEngine);

    // 向工具栏添加动作和部件
    ui->mainToolBar->addAction(view->pageAction(QWebEnginePage::Back));
    ui->mainToolBar->addAction(view->pageAction(QWebEnginePage::Forward));
    ui->mainToolBar->addAction(view->pageAction(QWebEnginePage::Reload));
    ui->mainToolBar->addAction(view->pageAction(QWebEnginePage::Stop));
    ui->mainToolBar->addWidget(webSiteIconLabel);
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
    locationEdit->setCursorPosition(0);
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

// 获取图标文件
void MainWindow::handleIconUrlChanged(const QUrl &url)
{
    QNetworkRequest iconRequest(url);
    QNetworkReply *iconReply = manager->get(iconRequest);
    iconReply->setParent(this);
    connect(iconReply, &QNetworkReply::finished, this, &MainWindow::handleIconLoaded);
}

// 对获取的图标文件进行处理
void MainWindow::handleIconLoaded()
{
    QPixmap pixmap;
    QNetworkReply *iconReply = qobject_cast<QNetworkReply*>(sender());
    if (iconReply && iconReply->error() == QNetworkReply::NoError) {
        QByteArray data = iconReply->readAll();
        pixmap.loadFromData(data);
        webSiteIcon->addPixmap(pixmap);
        iconReply->deleteLater();
    } else {
        QImage image(QString(":/image/internet-web-browser.png"));
        pixmap = QPixmap::fromImage(
                    image.scaled(
                        18, 18,
                        Qt::KeepAspectRatio,
                        Qt::SmoothTransformation));
    }
    webSiteIconLabel->setPixmap(pixmap);
}
