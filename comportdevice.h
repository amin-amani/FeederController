#ifndef COMPRTDEVICE_H
#define COMPRTDEVICE_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QDebug>
#include <QEventLoop>
#include <QTimer>
#include <QSignalSpy>

class ComportDevice : public QObject
{
    Q_OBJECT
    QSerialPort _comport;

public:
    QByteArray _reply;
    explicit ComportDevice(QObject *parent = nullptr);
    bool Init(QString portName,int baudRate);
    QByteArray SendCommand(QByteArray data);

    QByteArray ReadAll();
    QByteArray ModbusCRC(QByteArray data);
    bool IsOpen();
    void Close();
signals:
    void PacketCompleted(QByteArray);

public slots:

       void ReadyRead();


};

#endif // COMPRTDEVICE_H
