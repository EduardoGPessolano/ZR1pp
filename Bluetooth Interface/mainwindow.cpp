#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QComboBox>

#define MYARDUINO_ADDRESS "00:20:12:08:BA:44"
#define MYARDUINO_NAME "hc05-arduino-nano"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    QWebEngineView *view = new QWebEngineView(this->ui->centralwidget);

//    view->load(QUrl("http://192.168.18.90"));
//    view->show();

    // Check if Bluetooth is available on this device

    if (!localDevice.isValid())
    {
        QMessageBox::critical(this, "Error: ","Could not fetch bluetooth locally");
        return;
    }

    // Turn Bluetooth on
    localDevice.powerOn();

    // Read local device name
    localDeviceName = localDevice.name();

    // Make it visible to others
    localDevice.setHostMode(QBluetoothLocalDevice::HostDiscoverable);

    // Create a discovery agent and connect to its signals
    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(discoveryAgent, SIGNAL(deviceDiscovered(QBluetoothDeviceInfo)),
                this, SLOT(deviceDiscovered(QBluetoothDeviceInfo)));

    // Start a discovery
    discoveryAgent->start();

    // Get connected devices, not used here!
    remotes = localDevice.connectedDevices();

    // Connect to service
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);
    connect(socket, SIGNAL(connected()), this, SLOT(socketConnected()));
    connect(socket, SIGNAL(readyRead()), this, SLOT(dataReady2Read()));

    //Cursor is busy
    this->setCursor(Qt::BusyCursor);

    // disable Buttons
    ui->drive_mode_label->setEnabled(false);
    ui->lights_mode_comboBox->setEnabled(false);
    ui->forward_pushButton->setEnabled(false);
    ui->backwards_pushButton->setEnabled(false);
    ui->rightBackwards_pushButton->setEnabled(false);
    ui->leftBackwards_pushButton->setEnabled(false);
    ui->rightForward_pushButton->setEnabled(false);
    ui->leftForward_pushButton->setEnabled(false);
//    ui->right_pushButton->setEnabled(false);
//    ui->left_pushButton->setEnabled(false);
    ui->progressBar->setEnabled(false);
}


//bluetooth device discovery
void MainWindow::deviceDiscovered(const QBluetoothDeviceInfo device)
{
    qDebug() << "Found new device:" << device.name() << '(' << device.address().toString() << ')';
    //QString br ="Found: "+device.address().toString();
    //ui->textEdit->append(br);                   // show on interface

    if (localDevice.pairingStatus(device.address())== QBluetoothLocalDevice::Paired)qDebug()<<"paired";

    if (device.address().toString()!= MYARDUINO_ADDRESS) return;

    // this is my Arduino device
    socket->connectToService(device.address(), 1); //Android

    //socket->connectToService(device.address(),QBluetoothUuid(QBluetoothUuid::SerialPort));//Desktop

    socket->open(QIODevice::ReadWrite);
    if (socket->isOpen())
    {          // write on status bar
        ui->statusBar->showMessage(("Device address: "+device.address().toString()), 3000);
    }

    this->setCursor(Qt::ArrowCursor);
}

//____________________________________________________________________________________________________
//   SLOT when data ready on bluetooth socket
void MainWindow::dataReady2Read()
{
    QByteArray data;
    data = socket->readAll();
    QString data2 = tr(data);
    ui->progressBar->setValue(data2.toDouble());

    // show on interface
    //ui->textEdit->append("receive:"+QString(data));
}

//____________________________________________________________________________________________________
//  SLOT when socket Connected
void MainWindow::socketConnected(){
   // show on interface
   socket->write("A\n");
   // enable Buttons
   ui->statusBar->showMessage(("Connected to: ZR1++"), 2000);
   ui->progressBar->setEnabled(true);
   ui->drive_mode_label->setEnabled(true);
   ui->lights_mode_comboBox->setEnabled(true);
   ui->forward_pushButton->setEnabled(true);
   ui->backwards_pushButton->setEnabled(true);
   ui->rightBackwards_pushButton->setEnabled(true);
   ui->leftBackwards_pushButton->setEnabled(true);
   ui->rightForward_pushButton->setEnabled(true);
   ui->leftForward_pushButton->setEnabled(true);
//   ui->right_pushButton->setEnabled(true);
//   ui->left_pushButton->setEnabled(true);
}

//____________________________________________________________________________________________________
//  destructor
MainWindow::~MainWindow()
{
    if (socket && socket->isOpen())
    {
        socket->close();
        delete socket;
    }
    delete ui;
}

//____________________________________________________________________________________________________
// SLOT buttons are being pressed or released
void MainWindow::on_forward_pushButton_pressed()
{

    if (!socket->isOpen() || !socket->isWritable())return;

    socket->write("F\n");
    //ui->textEdit->append("Send: F");
    ui->drive_mode_label->setText("D");
}

void MainWindow::on_forward_pushButton_released()
{
    if (!socket->isOpen() || !socket->isWritable())return;

    socket->write("S\n");
    //ui->textEdit->append("Send: S");
    ui->drive_mode_label->setText("N");
}

void MainWindow::on_backwards_pushButton_pressed()
{

    if (!socket->isOpen() || !socket->isWritable())return;

    socket->write("B\n");
    //ui->textEdit->append("Send: B");
    ui->drive_mode_label->setText("R");
}

void MainWindow::on_backwards_pushButton_released()
{
    if (!socket->isOpen() || !socket->isWritable())return;

    socket->write("S\n");
    //ui->textEdit->append("Send: S");
    ui->drive_mode_label->setText("N");
}


//Previous left button
//void MainWindow::on_left_pushButton_pressed()
//{

//    if (!socket->isOpen() || !socket->isWritable())return;

//    socket->write("L\n");
//    //ui->textEdit->append("Send: L");
//    ui->drive_mode_label->setText("D");
//}

//void MainWindow::on_left_pushButton_released()
//{
//    if (!socket->isOpen() || !socket->isWritable())return;

//    socket->write("S\n");
//    //ui->textEdit->append("Send: S");
//    ui->drive_mode_label->setText("N");
//}


//Previous right button
//void MainWindow::on_right_pushButton_pressed()
//{

//    if (!socket->isOpen() || !socket->isWritable())return;

//    socket->write("R\n");
//    //ui->textEdit->append("Send: R");
//    ui->drive_mode_label->setText("D");
//}

//void MainWindow::on_right_pushButton_released()
//{
//    if (!socket->isOpen() || !socket->isWritable())return;

//    socket->write("S\n");
//    //ui->textEdit->append("Send: S");
//    ui->drive_mode_label->setText("N");
//}

//____________________________________________________________________________________________________
//  SLOT when lights modes are being changed

void MainWindow::on_lights_mode_comboBox_currentTextChanged(const QString &mode)
{
    ui->statusBar->showMessage(("Headlights mode: "+ mode), 2000);

    if(mode== "ON")
         socket->write("Y\n");

    if(mode == "OFF")
         socket->write("N\n");

    if(mode == "AUTO")
        socket->write("A\n");
}

//Left Forward
void MainWindow::on_leftForward_pushButton_pressed()
{
        if (!socket->isOpen() || !socket->isWritable())return;

        socket->write("Q\n");
        //ui->textEdit->append("Send: L");
        ui->drive_mode_label->setText("D");
}

void MainWindow::on_leftForward_pushButton_released()
{
        if (!socket->isOpen() || !socket->isWritable())return;

        socket->write("S\n");
        //ui->textEdit->append("Send: S");
        ui->drive_mode_label->setText("N");
}

//Right Forward
void MainWindow::on_rightForward_pushButton_pressed()
{
    if (!socket->isOpen() || !socket->isWritable())return;

    socket->write("E\n");
    //ui->textEdit->append("Send: L");
    ui->drive_mode_label->setText("D");
}

void MainWindow::on_rightForward_pushButton_released()
{
    if (!socket->isOpen() || !socket->isWritable())return;

    socket->write("S\n");
    //ui->textEdit->append("Send: S");
    ui->drive_mode_label->setText("N");
}

//Left Backwards
void MainWindow::on_leftBackwards_pushButton_pressed()
{
    if (!socket->isOpen() || !socket->isWritable())return;

    socket->write("Z\n");
    //ui->textEdit->append("Send: L");
    ui->drive_mode_label->setText("R");
}

void MainWindow::on_leftBackwards_pushButton_released()
{
    if (!socket->isOpen() || !socket->isWritable())return;

    socket->write("S\n");
    //ui->textEdit->append("Send: S");
    ui->drive_mode_label->setText("N");
}

//Right Backwards
void MainWindow::on_rightBackwards_pushButton_pressed()
{
    if (!socket->isOpen() || !socket->isWritable())return;

    socket->write("C\n");
    //ui->textEdit->append("Send: L");
    ui->drive_mode_label->setText("R");
}

void MainWindow::on_rightBackwards_pushButton_released()
{
    if (!socket->isOpen() || !socket->isWritable())return;

    socket->write("S\n");
    //ui->textEdit->append("Send: S");
    ui->drive_mode_label->setText("N");
}

