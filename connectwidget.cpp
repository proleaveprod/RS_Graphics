#include "connectwidget.h"
#include "ui_connectwidget.h"
#include <QMessageBox>



QStringList com_ports = {"COM1","COM2","COM3","COM4","COMn"};

connectWidget::connectWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::connectWidget)
{
    ui->setupUi(this);

    ui->COMlistWidget->addItems(com_ports);

}

connectWidget::~connectWidget()
{
    delete ui;
}

void connectWidget::on_connectButton_clicked()
{
    qDebug()<<"Connect to"<< ui->COMlistWidget->currentItem()->text();
    connectWidget::close();
    //QApplication::exit();
}


void connectWidget::on_updateButton_clicked()
{
    QMessageBox::about(this,"UPDATE","COMS UPDATE");
}

