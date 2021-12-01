#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
// necessary for bluetooth
#include <QBluetoothLocalDevice>
#include <QBluetoothAddress>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothServer>
#include <QListWidget>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void deviceDiscovered(QBluetoothDeviceInfo);

private slots:
    void socketConnected();
    void dataReady2Read();
    void on_forward_pushButton_pressed();
    void on_forward_pushButton_released();
    void on_backwards_pushButton_pressed();
    void on_backwards_pushButton_released();
    void on_left_pushButton_released();
    void on_left_pushButton_pressed();
    void on_right_pushButton_pressed();
    void on_right_pushButton_released();
    void on_lights_mode_comboBox_currentTextChanged(const QString &mode);


private:
    Ui::MainWindow *ui;
    QBluetoothLocalDevice localDevice;                  // the local interface
    QString localDeviceName;
    QList<QBluetoothAddress> remotes;
    QBluetoothDeviceDiscoveryAgent *discoveryAgent;
    QBluetoothSocket *socket;                           // a socket for communication
};

#endif // MAINWINDOW_H
