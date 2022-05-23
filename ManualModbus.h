#ifndef MANULMODBUS_H
#define MANULMODBUS_H

#include <QObject>
#include <QEventLoop>
#include <QTimer>
#include "Types.h"
#include "comportdevice.h"
#include <QFile>
#include <QThread>
#include <QMutexLocker>
#include <QWaitCondition>
class ManualModbus : public QObject
{
    Q_OBJECT

    QString _portName;

    QMutex _meteringMutex;
    QWaitCondition _meteringWait;
    int _baudeRate;
    ComportDevice *_comport=nullptr;
    QFile *_gpioFile;
    QByteArray ModbusCRC(QByteArray data);
    QByteArray CreateModbusWritePacket(int id, int registerType, int registerAddress, int value);
    ModbusReadingParameters ReadModbusValues();
public:
    enum AlarmColor
    {     Green=0,
          Red=1,
          Yellow=2,
          RedGreen,
          Off
    };
    enum class ModbusRegisterTypes
    {
        Reading=0x04,
        holding=0x06

    };
    enum class SorterRegisters
    {
        FeerderRatio=0x00,
        PCPowerOff=0x0D,
        PCPowerOn=0x0E,
        ProcessorTemp=0x04,
        PresureSensor=0x05,
        TopCamera1=0x00,
        TopCamera2=0x02,
        BottomCamera1=0x01,
        BottomCamera2=0x03
    };

    explicit ManualModbus(QObject *parent = nullptr);

    explicit ManualModbus(QString portName , int baudRate, int updateRate=1000);
    void WaitMs(int ms);

    void SetDirection(bool status);

    int16_t ReadParameter(int id, int registerType, int addres, int val);
    QByteArray SendCommand(QByteArray packet, int timeout);
    QByteArray SendCommands(QByteArray packet, int timeout = 10, int retry = 1);
    bool Init(QString name, int baudRate);
public slots:
//    void Start();
    bool SetFeederSpeed(int chuteID, int speed);
    bool SetFeederPower(int chuteID, int state);
    bool SetChuteAlarm(AlarmColor color, int value);
    void SetPCPower(int chuteID,int status);
    void SerialPacketCompleted(QByteArray data);

signals:

    void ReadyRead(ModbusReadingParameters values);
    void ErrorConnection(int errorNumber);
    void ModBusError();


};

#endif // MANULMODBUS_H
