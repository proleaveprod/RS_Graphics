#ifndef MAINWINDOW_H
#define MAINWINDOW_H




#include <QMainWindow>
#include <connectwidget.h>
#include <QDebug>
#include "qcustomplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void clickedGraph(QMouseEvent *event);
    void mouseMoved(QMouseEvent *eventMouseMoved);

    void on_actionMakeGraph_triggered();

    void on_actionClearGraph_triggered();




    void on_actionMakeAconnection_triggered();

private:
    Ui::MainWindow *ui;

    QCPCurve *verticalLine;     // Объявляем объект для вертикальной линии
    QCPItemTracer *tracer;      // Трасировщик по точкам графика


};
#endif // MAINWINDOW_H
