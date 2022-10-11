#ifndef MAINWINDOW_H
#define MAINWINDOW_H




#include <QMainWindow>
#include <connectwidget.h>
#include <QDebug>
#include "qcustomplot.h"
#include <QSerialPort>



#define MAX_N   10000


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


public slots:
    void SlotsignalConnected();
    void on_readSerial();

private slots:

    void clickedGraph(QMouseEvent *event);
    void mouseMoved(QMouseEvent *eventMouseMoved);
    void keyPressEvent(QKeyEvent *eventKeypress);

    void on_actionMakeGraph_triggered();

    void on_actionClearGraph_triggered();






    void on_actionMakeAconnection_triggered();

private:
    Ui::MainWindow *ui;

    QCPCurve *verticalLine;     // Объявляем объект для вертикальной линии
    QCPItemTracer *tracer;      // Трасировщик по точкам графика
    QCPItemText *tracerLabel;
    QVector<double> x,y;

    void graphSetup();
    void graphMaker(QByteArray rec_data);



};
#endif // MAINWINDOW_H
