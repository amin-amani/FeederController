#include "mainwindow.h"

#include <QApplication>
//========================================================================================
void QtRegisterCustomTypes()
{
    qRegisterMetaType<ManualModbus::AlarmColor>("ManualModbus::AlarmColor");

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
