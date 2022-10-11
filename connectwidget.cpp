#include "connectwidget.h"
#include "ui_connectwidget.h"
#include "mainWindow.h"
#include <QMessageBox>
#include <QSerialPort>
#include <QSerialPortInfo>


QSerialPort serial;

QStringList com_ports = {"COM1","COM2","COM3","COM4","COMn"};



connectWidget::connectWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::connectWidget)
{
    ui->setupUi(this);

    //ui->COMlistWidget->addItems(com_ports)


    //qDebug() << "Number of serial ports:" << QSerialPortInfo::availablePorts().

        Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
            ui->COMlistWidget->addItem(port.portName());
        }



    //connect(this, &connectWidget::signalConnected, MainWindow, &MainWindow::SlotsignalConnected);


}

connectWidget::~connectWidget()
{
    delete ui;
}


void connectWidget::on_connectButton_clicked()
{


    if(serial.isOpen()){
        qDebug()<<"Serial is closing";
        serial.close();
     }

    cur_com = ui->COMlistWidget->currentItem()->text();
    qDebug()<<"Try to connect "<< cur_com;

    serial.setPortName(cur_com);
    serial.setBaudRate(QSerialPort::Baud115200);
     serial.setDataBits(QSerialPort::Data8);
     serial.setParity(QSerialPort::NoParity);
     serial.setStopBits(QSerialPort::OneStop);
     serial.setFlowControl(QSerialPort::NoFlowControl);
    if ( serial.open(QIODevice::ReadWrite)) {
        qDebug()<<"Connected:"<< cur_com;
        connectWidget::close();


    }else{
        QMessageBox::critical(this, tr("Ошибка"),  serial.errorString());
        qDebug()<<"Error connection";

    }
    serial.clear();

    //connectWidget::close();

    //QApplication::exit();
}


void connectWidget::on_updateButton_clicked()
{
    ui->COMlistWidget->clear();
    Q_FOREACH(QSerialPortInfo port, QSerialPortInfo::availablePorts()) {
        ui->COMlistWidget->addItem(port.portName());
        qDebug() << port.portName();
    }


}


