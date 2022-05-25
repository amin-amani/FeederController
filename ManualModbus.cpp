#include "ManualModbus.h"
//=================================================================================================
ManualModbus::ManualModbus(QObject *parent) : QObject(parent)
{



}
//=================================================================================================
ManualModbus::ManualModbus(QString portName, int baudRate, int updateRate)
{
    _portName=portName;
    _baudeRate=baudRate;
}
//==============================================================================
void  ManualModbus::SetDirection(bool status)
{
    if(!_gpioFile->isOpen())return;
    if(status)
    {
        //        qDebug() << "we set to write++++++++++++++++";
        _gpioFile->write("1"); //1
        _gpioFile->flush();
    }
    else {
        //        qDebug() << "we set to read------------------";
        _gpioFile->write("0"); //0
        _gpioFile->flush();

    }

}
//=================================================================================================
void ManualModbus::WaitMs(int ms)
{

    QEventLoop q;
    QTimer tT;
    tT.setSingleShot(true);
    connect(&tT, SIGNAL(timeout()), &q, SLOT(quit()));
    tT.start(ms);
    q.exec();
    if(tT.isActive()){
        tT.stop();
    } else
    {

    }
}
//=================================================================================================
bool ManualModbus::Init(QString name, int baudRate)
{


//    //    connect(_comport, SIGNAL(PacketCompleted( QByteArray)), this, SLOT(SerialPacketCompleted(QByteArray)));
//    _gpioFile=new QFile();
//    _gpioFile->setFileName("/sys/class/gpio/gpio232/value");
//    if(!_gpioFile->open(QFile::ReadWrite))
//    {
//        qDebug()<<"unable to open modbus write pin";

//    }
//    SetDirection(true);
//    return _comport->Init(name,baudRate);

//    while (true) {
//        WaitMs(1000);
//        bool lockStatus=_meteringMutex.tryLock(1000);
//        // qDebug()<<"can lock:"<<lockStatus;
//        if(lockStatus)
//        {
//        //    ModbusReadingParameters val=ReadModbusValues();
//         //   emit ReadyRead(val);
////            _meteringMutex.unlock();
////            _meteringWait.wakeAll();
//          //   qDebug()<<"metering done....";
//        }else
//        {

//            //qDebug()<<"metering pause....";
//        }




//    }
    return true;
}
//=================================================================================================

void ManualModbus::Start()
{
    //    connect(_comport, SIGNAL(PacketCompleted( QByteArray)), this, SLOT(SerialPacketCompleted(QByteArray)));
    _gpioFile=new QFile();
    _gpioFile->setFileName("/sys/class/gpio/gpio232/value");
    if(!_gpioFile->open(QFile::ReadWrite))
    {
       // qDebug()<<"unable to open modbus write pin";

    }
    _comport=new ComportDevice();
    SetDirection(true);
     if(!_comport->Init(_portName,_baudeRate))
     {
         emit ErrorConnection(0);
         //qDebug()<<"emit error connection";
         return;
     }
    _working=true;
    while (_working)
    {
          WaitMs(2);
    val=ReadModbusValues();
     emit ReadyRead(val);
//    qDebug()<<"!!!!!!!!!!!!!!!!!!="<<val.AirPressure;
    WaitMs(1000);

    }
    _comport->Close();
  //  qDebug()<<"closing port";
}

void ManualModbus::Stop()
{
    _working=false;
}

//=================================================================================================
QByteArray ManualModbus::ModbusCRC(QByteArray data)
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
//=================================================================================================
int16_t ManualModbus::ReadParameter(int id,int registerType,int addres,int val)
{
    int16_t result=0;
    QByteArray transmit=  CreateModbusWritePacket(id,registerType,addres,val);
    QByteArray rx=SendCommand(transmit,200);
    if(rx.length()<1){
        //  qDebug()<<"read error";
        return result;}
     else {
        //qDebug()<<"<--"<<rx.toHex();
    }
    result=(rx[3]&0xff);
    result<<=8;
    result+=(rx[4]&0xff);

    return result;
}
//=================================================================================================
ModbusReadingParameters ManualModbus::ReadModbusValues()
{
     ModbusReadingParameters result;
    static int statCount=0;
    if(_comport==nullptr)return result;
    if(!_comport->IsOpen())return result;
  result.AirPressure= ReadParameter(100,(uint8_t)ModbusRegisterTypes::Reading,(uint8_t)SorterRegisters::PresureSensor,0);

//     qDebug()<<"Air="<<result.AirPressure;
    result.TopCameraTemp1= ReadParameter(100,(uint8_t)ModbusRegisterTypes::Reading,(uint8_t)SorterRegisters::TopCamera1,1);
    //  qDebug()<<"TopCameraTemp1="<<result.TopCameraTemp1;
   result.TopCameraTemp2= ReadParameter(100,(uint8_t)ModbusRegisterTypes::Reading,(uint8_t)SorterRegisters::TopCamera2,1);
    //   qDebug()<<"TopCameraTemp2="<<result.TopCameraTemp2;
    result.ProcessorTemp= ReadParameter(100,(uint8_t)ModbusRegisterTypes::Reading,(uint8_t)SorterRegisters::ProcessorTemp,1);

    //qDebug()<<"ProcessorTemp="<<result.ProcessorTemp;
    result.BottomCameraTemp1= ReadParameter(100,(uint8_t)ModbusRegisterTypes::Reading,(uint8_t)SorterRegisters::BottomCamera1,1);
    // qDebug()<<"BottomCameraTemp1="<< result.BottomCameraTemp1;
    result.BottomCameraTemp2= ReadParameter(100,(uint8_t)ModbusRegisterTypes::Reading,(uint8_t)SorterRegisters::BottomCamera2,1);
    // qDebug()<<"BottomCameraTemp2="<< result.BottomCameraTemp2;
//emit ReadyRead(result);

  return  result;

}
//=================================================================================================
QByteArray ManualModbus::CreateModbusWritePacket(int id,int registerType,int registerAddress,int32_t value)
{
    QByteArray result;
    result.append(id&0xff);
    result.append(registerType&0xff);
    result.append((registerAddress&0xff00)>>8);
    result.append((registerAddress&0xff));
    result.append((value&0x00ff00)>>8);
    result.append((value&0xff));
    return  ModbusCRC(result);
}
//=================================================================================================
QByteArray ManualModbus::SendCommands(QByteArray packet,int timeout,int retry)
{
    QByteArray reply;
  //  for(int i=0;i<retry;retry++){

        reply= SendCommand(packet,timeout);
    //    if(reply.count()>=8)break;
//    }
return reply;


}
//=================================================================================================
QByteArray ManualModbus::SendCommand(QByteArray packet,int timeout=10)
{
//qDebug()<<"send.......................";
    QSignalSpy spy(_comport, SIGNAL(PacketCompleted(QByteArray)));
    WaitMs(100);
    QByteArray reply;
    SetDirection(true);
    spy.wait(2);
    _comport->SendCommand(packet);

    spy.wait(1);
    SetDirection(false);
    spy.wait(timeout);
    if(spy.count()<1)return reply;
    WaitMs(1);
    if(spy.count()>0)
    return (QByteArray) spy.takeFirst()[0].toByteArray();
    QByteArray empty;
    return empty;
}
//=================================================================================================
bool ManualModbus::SetFeederSpeed(int chuteID, int speed)
{
    //    qDebug()<<"set feeder SPEED start";
    if(_comport==nullptr)return false;
    if(!_comport->IsOpen())return false;
//speed=0x1d 00 1d 1e 1f 21 22 23;   01 06 00 00 00 1f c8 02
  //  qDebug()<<"set feeder SPEED "<<chuteID<<" speed="<<speed;
    if(_meteringMutex.tryLock(1000))
    {       QByteArray reply=SendCommands(CreateModbusWritePacket(chuteID,0x06,0,speed),20,1);
               // qDebug()<<"replay:"<<reply.toHex();
               // WaitMs(10);
                _meteringMutex.unlock();
                if(reply.length()<8)return false;



            }


    return true;


}
//=================================================================================================
bool ManualModbus::SetFeederPower(int chuteID, int state)
{
    QByteArray reply;
    if(_comport==nullptr)return false;
    if(!_comport->IsOpen())return false;
    if(_meteringMutex.tryLock(1000))
    {
        //qDebug()<<"set feeder power not busy: "<<state <<" "<<QThread::currentThreadId(); ;
        if(state==0)
        {
            reply=SendCommands(CreateModbusWritePacket(chuteID,0x06,0x01,0x0001),10);
          //  qDebug()<<"replay:"<<_comport->_reply.toHex();

        }
        else
        {
            reply=SendCommands(CreateModbusWritePacket(chuteID,0x06,0x01,0x0000),10);
           // qDebug()<<"replay:"<<_comport->_reply.toHex();
        }
        _meteringMutex.unlock();
   }
//    else
//    {
//        qDebug()<<"waite set feeder power : "<<state <<" "<<QThread::currentThreadId(); ;
//        _meteringWait.wait(&_meteringMutex, 7000);
//        _meteringMutex.tryLock();
//        if(state==0)
//        {
//            reply=SendCommand(CreateModbusWritePacket(chuteID,0x05,0x02,0xff00),10);
////            qDebug()<<"replay:"<<_comport->_reply.toHex();
//        }
//        else
//        {
//            reply=SendCommand(CreateModbusWritePacket(chuteID,0x05,0x02,0x0000),10);
////            qDebug()<<"replay:"<<_comport->_reply.toHex();
//        }
//        _meteringMutex.unlock();
//    }
    if(reply.count()<8)return false;
    return true;


}
//=================================================================================================
bool ManualModbus::SetChuteAlarm(ManualModbus::AlarmColor color,int value)
{

   // qDebug()<<"====================SetChuteAlarm clicked llll ";
    WaitMs(1);
    QByteArray reply;
    if(_meteringMutex.tryLock(1000))
    {

        // qDebug()<<"SetChuteAlarm not busy: "<<QThread::currentThreadId();
        if(color==AlarmColor::Green){

            reply+=SendCommands(CreateModbusWritePacket(100,0x06,2,value),100);
             //      qDebug()<<"replay:"<<_comport->_reply.toHex();
            //                reply+=SendCommand(CreateModbusWritePacket(100,0x06,1,0),100);
            //                reply+=SendCommand(CreateModbusWritePacket(100,0x06,2,0),100);
        }
        else if (color==AlarmColor::Red)
        {
            //                 reply+=SendCommand(CreateModbusWritePacket(100,0x06,0,0),100);
            //                 reply+=SendCommand(CreateModbusWritePacket(100,0x06,1,0),100);
            reply+=SendCommands(CreateModbusWritePacket(100,0x06,1,value),100);
                 //  qDebug()<<"replay:"<<_comport->_reply.toHex();
        }
        else {
            // reply+=SendCommand(CreateModbusWritePacket(100,0x06,0,0),100);
            reply+=SendCommands(CreateModbusWritePacket(100,0x06,0,value),100);
              //     qDebug()<<"replay:"<<_comport->_reply.toHex();
            // reply+=SendCommand(CreateModbusWritePacket(100,0x06,2,0),100);
        }
        _meteringMutex.unlock();

    }
    if(reply.count()<8)return false;
    return true;


}
//=================================================================================================
void ManualModbus::SetPCPower(int chuteID, int status)
{
    if(_meteringMutex.tryLock(400))

    {
        if(status==0){

            SendCommands(CreateModbusWritePacket(chuteID,0x06,(uint8_t)SorterRegisters::PCPowerOff,1),10);

        }
        else {

            SendCommands(CreateModbusWritePacket(chuteID,0x06,(uint8_t)SorterRegisters::PCPowerOn,1),10);


        }
       _meteringMutex.unlock();

}



}
//=================================================================================================
void ManualModbus::SerialPacketCompleted(QByteArray data)
{
    //qDebug()<<" ManulModbus::SerialPacketCompleted:"<<data.toHex();
}
//=================================================================================================

