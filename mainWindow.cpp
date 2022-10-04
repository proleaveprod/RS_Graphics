#include "mainWindow.h"
#include "ui_mainWindow.h"
#include "QMessageBox"
#include "QRandomGenerator"
#include "connectwidget.h"


#define MAX_N   10000

double amps[MAX_N] ={};

double xStart = 0; //Начало интервала, где рисуем график по оси Ox
double xEnd =  MAX_N-1; //Конец интервала, где рисуем график по оси Ox
double h = 1; //Шаг, с которым будем пробегать по оси Ox
uint8_t graph_n=0;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);
    connect(ui->widget,SIGNAL(mousePress(QMouseEvent*)),SLOT(clickedGraph(QMouseEvent*)));
     connect(ui->widget, SIGNAL(mouseMove(QMouseEvent*)),SLOT(mouseMoved(QMouseEvent*)));

    ui->widget->xAxis->setRange(0,MAX_N);
    ui->widget->yAxis->setRange(0, 300);//Для оси Oy
    ui->widget->xAxis->setLabel("N");
    ui->widget->yAxis->setLabel("Amplitude");
    ui->widget->setInteraction(QCP::iRangeZoom,true);
    ui->widget->setInteraction(QCP::iRangeDrag,true);
//ui->widget->setBackground(QBrush(QColor("#242423")));
    //ui->widget->legend->setTextColor("#CFDBD5");
    //ui->widget->legend->set

    verticalLine = new QCPCurve(ui->widget->xAxis, ui->widget->yAxis);



    tracer = new QCPItemTracer(ui->widget);
    tracer->setGraph(ui->widget->graph(0));   // Трассировщик будет работать с графиком
    QVector<double> x(2) , y(2);
            x[0] = 0;
            y[0] = -50;
            x[1] = 0;
            y[1] = 50;

    //ui->widget->graph(verticalLine);   // Добавляем линию на полотно
    //verticalLine->setName("Vertical");      // Устанавливаем ей наименование
    //verticalLine->setData(x, y);            // И устанавливаем координаты


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clickedGraph(QMouseEvent *event)
{
    int x_click =  ui->widget->xAxis->pixelToCoord(event->pos().x());
    double y_click =  ui->widget->yAxis->pixelToCoord(event->pos().y());
    QString coord_click = "x = " + QString::number(x_click) + "  y = " + QString::number(y_click,'f',3);
    ui->statusBar->showMessage(coord_click);

     tracer->setGraphKey(x_click);
     ui->lineEdit->setText("ewrwer: " + QString::number(tracer->position->key()) +
                           " y: " + QString::number(tracer->position->value()));
}

void MainWindow::mouseMoved(QMouseEvent *eventMouseMoved)
{
    ui->statusBar->showMessage("двигаешься");
}








void MainWindow::on_actionMakeGraph_triggered()
{

    QVector<double> x, y; //Массивы координат точек

    //##############СОЗДАНИЕ ПРОИЗВОЛЬНОЙ СИНУСОИДЫ################################
    int a = 10 + rand() % 1000;
    qDebug()<<"A = "<< a;
    double f = 1 + rand() % 100;
    f= f/321213;
    qDebug()<<"f = "<< f;
    double fi = rand() % 360;
    qDebug()<<"fi = "<< fi;
    fi = fi*M_PI/180;
    for(int i=0;i<MAX_N;i++)amps[i]=a*qSin(2*M_PI*f*i + fi);  // Вырисовка графика
    for (int i= xStart;i<=xEnd;i+=h){x.push_back(i);y.push_back(amps[i]);} // Отправка в массив
    //############################################################################

    ui->widget->addGraph(); //Добавляем один график в widget

    ui->widget->graph(graph_n)->setData(x, y); //Говорим, что отрисовать нужно график по нашим двум массивам x и y

    uint8_t r_color = rand();
    uint8_t g_color = rand();
    uint8_t b_color = rand();
    qDebug()<<"Color of line: "<< r_color << g_color << b_color;
    ui->widget->graph(graph_n)->setPen(QColor(r_color, g_color, b_color, 255));//задаем цвет точки
    ui->widget->graph(graph_n)->setLineStyle(QCPGraph::lsNone);//убираем линии
    ui->widget->graph(graph_n)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssDisc , 2));

    ui->widget->replot();


    ui->statusBar->showMessage("Cоздана кривая №"+QString::number(graph_n));
    graph_n++;
}


void MainWindow::on_actionClearGraph_triggered()
{
    ui->statusBar->showMessage("CLEAR A GRAPH");
    ui->widget->clearGraphs();
    graph_n=1;
}

void MainWindow::on_actionMakeAconnection_triggered()
{
    connectWidget connect_w;
    connect_w.setModal(true);
    connect_w.exec();

}

