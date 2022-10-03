#include "mainWindow.h"
#include "ui_mainWindow.h"
#include "QMessageBox"




MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);




}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_connectButton_clicked()
{








}

void MainWindow::on_updateButton_clicked() // Здесь будет находиться алгоритм обновления списка COM-портов.
{
    QMessageBox::about(this,"UPDATE","COMS UPDATE");
}




