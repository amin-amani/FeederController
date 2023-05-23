#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSerialPort/QSerialPortInfo>
#include <QSerialPort>
#include <QDebug>
#include "ManualModbus.h"
#include "Types.h"
#include <QThread>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    ManualModbus *_modbus=nullptr;
    QThread *_meteringTread=nullptr;



public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    QByteArray ModbusCRC(QByteArray data);
    QByteArray CreateModbusWritePacket(int id, int registerType, int registerAddress, int32_t value);

    QByteArray SendCommand(QByteArray packet, int timeout = 10);

    void SetSortButtonStat(bool stat);
signals:
   bool SetChuteAlarm(ManualModbus::AlarmColor color,int value);
   bool SetFeederSpeed(int chuteID, int speed);
   bool SetFeederPower(int chuteID, int state);
private slots:
    void ModbusParmetersReadyRead(ModbusReadingParameters values);
    void ModbusErrorConnection(int errorCode);
    void ComportRedyRead();
    void on_dial_sliderMoved(int position);

    void on_BtnPort_clicked();

    void on_BtnSetSpeed_clicked();

    void on_BtnSetFeederPower_clicked();

    void on_BtnSetAlarm_clicked();

    void on_BtnSetAlarm_3_clicked();

    void on_BtnSetAlarm_2_clicked();


    void on_BtnRefreshPort_clicked();

    void on_BtnStop_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
