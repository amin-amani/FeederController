#ifndef TYPES_H
#define TYPES_H
#include <QObject>

class ModbusReadingParameters
{

public:
    int TopCameraTemp1=0;
    int BottomCameraTemp1=0;
    int TopCameraTemp2=0;
    int BottomCameraTemp2=0;
    int ProcessorTemp=0;
    float AirPressure=0;
    int SW1=0;
    int SW2=0;
    int SWBox=0;

};
enum class ConnectionStatus
{

    Ok=0,
    FrontCameraError,
    RearCameraError,
    BothCameraError,
    SerialPortError,
    AirPressureError,
    ModbusError,
    UnsepecificError

};
enum class SortingStatus
{
    DisConnected=-1,
    Running=0,
    Idle=1,
    Error=2


};
enum class StartButtonState
{
    Red=0,
    Green=1,
    Disble=2

};
enum class SensorStatus
{
    Enable=0,
    Disable=1
};

//enum class PCPower
//{
//    PCOff=0,
//    PCOn=1
//};


/*
class ConnectionErrors
{
public :
    static int Ok;
    static int FrontCameraError;
    static int RearCameraError;
    static int BothCameraError;
    static int SerialPortError;
    static int AirPressureError;
    static int ModbusError;
    ConnectionErrors()
    {   Ok=0;
        FrontCameraError=1;
        RearCameraError=2;
        BothCameraError=3;
        SerialPortError=4;
        AirPressureError=5;
        ModbusError=6;
    }

};
class SortterStatus
{
public:
static int Running;
static  int Idle;
SortterStatus()
{
    Running=0;
     Idle=1;
}

};
class StartButtonState
{
public:
 static int Red;
 static int Green;
 StartButtonState()
 {
     Red=0;
     Green=1;


 }

};

 */


//        //// 0 => OK
//        /// 1 => Camera F: Error
//        /// 2 => Camera R: Error
//        /// 3 => Both Error

#endif // TYPES_H
