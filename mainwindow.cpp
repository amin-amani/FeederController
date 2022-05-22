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
        connect(&_comport,SIGNAL(readyRead()),this , SLOT(ComportRedyRead()));
    }
}
//=======================================================================================================
MainWindow::~MainWindow()
{
    delete ui;
}
//=======================================================================================================
void MainWindow::ComportRedyRead()
{
    qDebug()<<_comport.readAll().toHex();
}
//=======================================================================================================
void MainWindow::on_dial_sliderMoved(int position)
{
    ui->spinBox->setValue(position);
}

//=================================================================================================

//=======================================================================================================
void MainWindow::on_BtnPort_clicked()
{
    ui->statusbar->showMessage("...");
    if(ui->BtnPort->text()=="Open")
    {
        //_comport.setPortName(ui->CmbPortName->currentText());
        //_comport.setBaudRate(115200);
        _modbus.Init(ui->CmbPortName->currentText(),115200);
//        if(_comport.open(QSerialPort::ReadWrite))
//        {
           ui->BtnPort->setText("Close");
//        }
//        else
//        {
//            ui->statusbar->showMessage("error open port");
//        }
    }
    else
    {
    _comport.close();
    ui->statusbar->showMessage("port closed");
    ui->BtnPort->setText("Open");
    }
}

//=======================================================================================================

void MainWindow::on_BtnSetSpeed_clicked()
{
_modbus.SetFeederSpeed(ui->NumChute->value(),ui->spinBox->value());
}

void MainWindow::on_BtnSetFeederPower_clicked()
{
    static bool stat=false;
    stat=!stat;
   _modbus.SetFeederPower(ui->NumChute->value(),stat);
}
