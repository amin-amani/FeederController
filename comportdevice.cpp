#include "comportdevice.h"
//==================================================================================================
ComportDevice::ComportDevice(QObject *parent) : QObject(parent)
{

}
//==================================================================================================
QByteArray ComportDevice::ModbusCRC(QByteArray data)
{
    unsigned int crc = 0xFFFF;
    int pos=0;
    int i;
    for (pos = 0; pos < data.length(); pos++)
    {
        crc ^= ((unsigned int)data[pos]&0x00FF);          // XOR byte into least sig. byte of crc

        for (i = 8; i != 0; i--)
        {    // Loop over each bit
            if ((crc & 0x0001) != 0)
            {      // If the LSB is set
                crc >>= 1;                    // Shift right and XOR 0xA001
                crc ^= 0xA001;
            }
            else                            // Else LSB is not set
                crc >>= 1;                    // Just shift right
        }
    }
    data.append(crc&0xff);
    data.append((crc&0xff00)>>8);

    // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
    return data;
}
//==================================================================================================
bool ComportDevice::Init(QString portName, int baudRate)
{
    _comport.setPortName(portName);
    _comport.setBaudRate(baudRate);
    connect(&_comport,SIGNAL(readyRead()),this,SLOT(ReadyRead()));
   if( _comport.open(QSerialPort::ReadWrite))
   {
       return true;


   }
   else
   {
       return  false;
   }
}
//==================================================================================================
QByteArray ComportDevice::SendCommand(QByteArray data)
{
    QByteArray reply;
    if(!_comport.isOpen())return reply;
    _comport.readAll();
    _reply.clear();
    qDebug()<<"-->>>>>>>"<<data.toHex();
    _comport.write(data);
    _comport.flush();



    return reply;

}
//==================================================================================================
QByteArray ComportDevice::ReadAll()
{
    return  _comport.readAll();

}
//==================================================================================================
void ComportDevice::ReadyRead()
{
_reply+=_comport.readAll();
if(_reply.length()<4)return;
if(ModbusCRC(_reply.mid(0,_reply.length()-2))==_reply)
emit PacketCompleted(_reply);

}
//==================================================================================================
