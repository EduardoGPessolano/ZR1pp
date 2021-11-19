#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtBluetooth>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWebEngineView *view = new QWebEngineView(this->ui->centralwidget);

    view->load(QUrl("http://192.168.18.90"));
    view->show();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{

}
