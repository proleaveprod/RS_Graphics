#ifndef CONNECTWIDGET_H
#define CONNECTWIDGET_H

#include <QDialog>
#include <mainWindow.h>
#include <QtSerialPort>




namespace Ui {
class connectWidget;
class ReceiverThread;
}

class connectWidget : public QDialog
{
    Q_OBJECT

public:
    explicit connectWidget(QWidget *parent = nullptr);
    ~connectWidget();
    QString cur_com;


signals:
    void signalConnected();

private slots:
    void on_connectButton_clicked();

    void on_updateButton_clicked();

private:
    Ui::connectWidget *ui;

    QSerialPortInfo *serialInfo;

};





#endif // CONNECTWIDGET_H
