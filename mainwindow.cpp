#include "mainwindow.h"
#include "ui_mainwindow.h"
//=======================================================================================================
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    for (QSerialPortInfo port : QSerialPortInfo::availablePorts())
    {
        //Their is some sorting to do for just list the port I want, with vendor Id & product Id
        ui->CmbPortName->addItem(port.portName());
        //ports.append(port.portName());

    }
SetSortButtonStat(false);



}
//=======================================================================================================
MainWindow::~MainWindow()
{
    delete ui;
}
//=======================================================================================================

void MainWindow::ModbusParmetersReadyRead(ModbusReadingParameters values)
{
//  qDebug()<<"modbus readyread:"<< values.AirPressure;
  ui->LabCameraTemp1->setText(QString::number( values.TopCameraTemp2));
  ui->LabCameraTemp2->setText(QString::number( values.TopCameraTemp2));
  ui->LabProcessorTemp->setText(QString::number( values.ProcessorTemp));
  ui->LabAirPressure->setText(QString::number( values.AirPressure));
}
//=======================================================================================================

void MainWindow::ModbusErrorConnection(int errorCode)
{
qDebug()<<"error code:"<<errorCode;
}
//=======================================================================================================
void MainWindow::ComportRedyRead()
{
//    qDebug()<<_comport.readAll().toHex();
}
//=======================================================================================================
void MainWindow::on_dial_sliderMoved(int position)
{
    ui->spinBox->setValue(position);
}
//=======================================================================================================
void MainWindow::SetSortButtonStat(bool stat)
{

      ui->BtnSetAlarm->setEnabled(stat);
      ui->BtnStop->setEnabled(stat);
      ui->BtnSetAlarm_2->setEnabled(stat);
      ui->BtnSetAlarm_3->setEnabled(stat);
      ui->BtnSetSpeed->setEnabled(stat);
      ui->BtnSetFeederPower->setEnabled(stat);
}

//=======================================================================================================
void MainWindow::on_BtnPort_clicked()
{
    ui->statusbar->showMessage("...");
//    qDebug()<<"...";
    if(ui->BtnPort->text()=="Open")
    {
        _meteringTread =new QThread();
        _modbus=new ManualModbus(ui->CmbPortName->currentText(),115200);
        connect(_meteringTread,SIGNAL(started()),_modbus,SLOT(Start()),Qt::UniqueConnection);
        connect(_meteringTread,SIGNAL(finished()), _meteringTread, SLOT(deleteLater()),Qt::UniqueConnection);
        connect(_meteringTread,SIGNAL(finished()), _meteringTread, SLOT(quit()),Qt::UniqueConnection);
        connect(_meteringTread, SIGNAL(finished()), _modbus, SLOT(deleteLater()),Qt::UniqueConnection);


        connect(this, SIGNAL(SetChuteAlarm(ManualModbus::AlarmColor,int)), _modbus, SLOT(SetChuteAlarm(ManualModbus::AlarmColor,int)),Qt::QueuedConnection);
        connect(this, SIGNAL(SetFeederPower(int , int )), _modbus, SLOT(SetFeederPower(int , int )),Qt::QueuedConnection);
          connect(this, SIGNAL(ValveConfig(QByteArray )), _modbus, SLOT(ValveConfig(QByteArray )),Qt::QueuedConnection);//aa
        connect(this, SIGNAL(SetFeederSpeed(int , int )), _modbus, SLOT(SetFeederSpeed(int , int )),Qt::QueuedConnection);////
        _modbus->moveToThread(_meteringTread);
        _meteringTread->start();

        ///
        connect(_modbus,SIGNAL(ReadyRead(ModbusReadingParameters)),this,SLOT(ModbusParmetersReadyRead(ModbusReadingParameters)),Qt::DirectConnection);
        connect(_modbus,SIGNAL(ErrorConnection(int)),this,SLOT(ModbusErrorConnection(int)),Qt::UniqueConnection);
        ///
        ui->BtnPort->setText("Close");
        SetSortButtonStat(true);


    }
    else
    {
    _meteringTread->quit();
    _modbus->Stop();
    _meteringTread=nullptr;
    _modbus=nullptr;
    SetSortButtonStat(false);

    ui->statusbar->showMessage("port closed");
    ui->BtnPort->setText("Open");
     // qDebug()<<"closing...";
    }
}

//=======================================================================================================

void MainWindow::on_BtnSetSpeed_clicked()
{

    emit SetFeederSpeed(ui->NumChute->value(),ui->spinBox->value());
    //    if( !emit SetFeederSpeed(ui->NumChute->value(),ui->spinBox->value()))
//    {
//     ui->statusbar->showMessage("communication error check connection!");
//    }

}
//=================================================================================================
void MainWindow::on_BtnSetFeederPower_clicked()
{
    static bool stat=false;
    stat=!stat;
   emit SetFeederPower(ui->NumChute->value(),stat);
}
//=================================================================================================

void MainWindow::on_BtnSetAlarm_clicked()
{

    static bool stat=false;
    stat=!stat;
    //qDebug()<<"set chute alarm";
       emit this->SetChuteAlarm(ManualModbus::AlarmColor::Red,stat);

}
//=================================================================================================


void MainWindow::on_BtnSetAlarm_2_clicked()
{
    static bool stat=false;
    stat=!stat;
        emit SetChuteAlarm(ManualModbus::AlarmColor::Green,stat);
       }
//=================================================================================================

void MainWindow::on_BtnSetAlarm_3_clicked()
{
    static bool stat=false;
    stat=!stat;
    emit SetChuteAlarm(ManualModbus::AlarmColor::Yellow,stat);
}
//=================================================================================================


//=================================================================================================

void MainWindow::on_BtnRefreshPort_clicked()
{
    ui->CmbPortName->clear();
    for (QSerialPortInfo port : QSerialPortInfo::availablePorts())
    {
        //Their is some sorting to do for just list the port I want, with vendor Id & product Id
        ui->CmbPortName->addItem(port.portName());
        //ports.append(port.portName());

    }
}

void MainWindow::on_BtnStop_clicked()
{
    ui->dial->setValue(0);
    ui->spinBox->setValue(0);
    emit SetFeederSpeed(ui->NumChute->value(),ui->spinBox->value());

}

void MainWindow::on_NumValveDelay_valueChanged(int arg1)
{

}

void MainWindow::on_NumValveDuration_valueChanged(int arg1)
{

//    QByteArray data;
//    data.append(0x01);
//    data.append(0x06);
//    data.append((char)0x00);
//    data.append(0x05);
//    data.append(arg1>>8);
//    data.append(arg1);
//    emit ValveConfig(data);
}

void MainWindow::on_NumValveDelay_editingFinished()
{
    QByteArray data;
   // 0x01	0x06	0x00	0x06	Value	Value	CRC1	CRC2
    data.append(0x01);
    data.append(0x06);
    data.append((char)0x00);
    data.append(0x06);
    data.append(ui->NumValveDelay->value()>>8);
    data.append(ui->NumValveDelay->value());
    qDebug()<<data.toHex();
     emit ValveConfig(data);
}

void MainWindow::on_NumValveDuration_editingFinished()
{
    QByteArray data;
   // 0x01	0x06	0x00	0x05	Value	Value	CRC1	CRC2
    data.append(0x01);
    data.append(0x06);
    data.append((char)0x00);
    data.append(0x05);
    data.append(ui->NumValveDuration->value()>>8);
    data.append(ui->NumValveDuration->value());
    qDebug()<<data.toHex();
     emit ValveConfig(data);
}
