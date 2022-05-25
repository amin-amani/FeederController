#include "TestObject.h"

TestObject::TestObject(QObject *parent) : QObject(parent)
{

}

void TestObject::Start()
{
    while(true){

        qDebug()<<"test class";
        QThread::msleep(1000);
        emit testSignal(ManualModbus::AlarmColor::Red,1);

    }
}
