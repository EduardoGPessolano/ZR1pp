#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QMainWindow>
#include<QBluetoothDeviceDiscoveryAgent>
#include<QBluetoothSocket>
#include<QListWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private slots:
    void on_find_clicked();
    void on_on_clicked();
    void on_off_clicked();
    void deviceDiscovered(const QBluetoothDeviceInfo &device);
    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    QBluetoothDeviceDiscoveryAgent *agent = new QBluetoothDeviceDiscoveryAgent;
    QBluetoothSocket *socket;
    QString string;
   bool k = false;
};


#endif // MAINWINDOW_H
