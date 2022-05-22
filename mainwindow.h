#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPortInfo>
#include <QSerialPort>
#include <QDebug>
#include "ManualModbus.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT
    QSerialPort _comport;
    ManualModbus _modbus;

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QByteArray ModbusCRC(QByteArray data);
    QByteArray CreateModbusWritePacket(int id, int registerType, int registerAddress, int32_t value);
    bool SetFeederSpeed(int chuteID, int speed);
    QByteArray SendCommand(QByteArray packet, int timeout = 10);
private slots:
    void ComportRedyRead();
    void on_dial_sliderMoved(int position);

    void on_BtnPort_clicked();

    void on_BtnSetSpeed_clicked();

    void on_BtnSetFeederPower_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
