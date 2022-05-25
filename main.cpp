#include "mainwindow.h"

#include <QApplication>
//========================================================================================
void QtRegisterCustomTypes()
{
    qRegisterMetaType<ManualModbus::AlarmColor>("ManualModbus::AlarmColor");
      qRegisterMetaType<ManualModbus::AlarmColor>("ModbusReadingParameters");

}
//========================================================================================
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QtRegisterCustomTypes();
    MainWindow w;
    w.show();
    return a.exec();
}
