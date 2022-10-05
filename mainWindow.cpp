#include "mainWindow.h"
#include "ui_mainWindow.h"
#include "QMessageBox"
#include "QRandomGenerator"
#include "connectwidget.h"
#include "QKeyEvent"


#define     BlackCLR       "#242423"
#define     GrayCLR        "#333533"
#define     YellowCLR      "#F5CB5C"
#define     OrangeCLR      "#E15A20"
#define     WhiteCLR       "#E8EDDF"
#define     LightGrayCLR   "#CFDBD5"

#define MAX_N   10000

double amps[MAX_N] ={};

double xStart = 0; //Начало интервала, где рисуем график по оси Ox
double xEnd =  MAX_N-1; //Конец интервала, где рисуем график по оси Ox
double h = 1; //Шаг, с которым будем пробегать по оси Ox
uint8_t graph_n,cur_graph;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);

//    QScreen *screen = QGuiApplication::primaryScreen();   // Алгоритм получения размера окна
//    QRect screenGeometry = screen->geometry();
//    int screenHeight = screenGeometry.height();
//    int screenWidth = screenGeometry.width();

    MainWindow::showMaximized();  // Разворачивание окна



    connect(ui->widget,SIGNAL(mousePress(QMouseEvent*)),SLOT(clickedGraph(QMouseEvent*)));
     connect(ui->widget, SIGNAL(mouseMove(QMouseEvent*)),SLOT(mouseMoved(QMouseEvent*)));

    ui->widget->setBackground(QColor(GrayCLR));


    ui->widget->xAxis->setRange(0,MAX_N);
    ui->widget->yAxis->setRange(0, 300);//Для оси Oy
    ui->widget->xAxis->setLabel("N");
    ui->widget->yAxis->setLabel("Amplitude");



    ui->widget->xAxis->grid()->setPen(QPen(QColor("black"), 1, Qt::DotLine));
    ui->widget->yAxis->grid()->setPen(QPen(QColor("black"), 1, Qt::DotLine));

    ui->widget->xAxis->setLabelColor(QColor(YellowCLR));
    ui->widget->yAxis->setLabelColor(QColor(YellowCLR));

    ui->widget->xAxis->setTickLabelColor(QColor(YellowCLR));
    ui->widget->yAxis->setTickLabelColor(QColor(YellowCLR));

    ui->widget->setInteraction(QCP::iRangeZoom,true);
    ui->widget->setInteraction(QCP::iRangeDrag,true);



    tracer = new QCPItemTracer(ui->widget);
        //tracer->setBrush(QBrush(Qt::red));
    tracer->setPen(QPen(QColor(YellowCLR), 1, Qt:: SolidLine));
    tracer->setStyle(QCPItemTracer::tsCrosshair);
    tracer->setSize(1.0);

    tracerLabel = new QCPItemText(ui->widget); // Генерация описания курсора
            // Следующий код предназначен для установки внешнего вида и выравнивания описания курсора
    tracerLabel->setLayer("overlay");
    //tracerLabel->setPen(QPen(OrangeCLR));
    tracerLabel->setPadding(QMargins(10,10,10,10));
    tracerLabel->setPositionAlignment(Qt::AlignLeft | Qt::AlignBottom);
    tracerLabel->setText(" ");
    tracerLabel->setColor(QColor(YellowCLR));
            // Следующий оператор очень важен, он привязывает описание курсора к позиции трассировщика для достижения автоматического следования
    tracerLabel->position->setParentAnchor(tracer->position);






}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::clickedGraph(QMouseEvent *event)
{

    int x_click =  ui->widget->xAxis->pixelToCoord(event->pos().x());

    if(x_click>xEnd)x_click=xEnd;       // Чтобы не вылезти за пределы кривой
    if(x_click<xStart)x_click=xStart;

//    double y_click = amps[x_click];

    tracer->setGraph(ui->widget->graph(cur_graph)); // Подключаем курсор к слою кривой
    tracer->setGraphKey(x_click); // Установить абсциссу курсора (ключ) на только что полученные данные абсциссы x
    tracer->setInterpolating(true); // Ордината курсора может быть автоматически получена линейной интерполяцией данных кривой (это не нужно рассчитывать вручную)
    tracer->updatePosition();


    double y_click = tracer->position->value();
    tracerLabel->setText(QString("x = %1, y = %2").arg(x_click).arg(y_click));


//    tracer->position->setCoords(x_click, y_click);
//    tracerLabel->setText(QString("x = %1, y = %2 (graph%3)").arg(x_click).arg(y_click).arg(cur_graph+1));
    ui->widget->replot();


}

void MainWindow::mouseMoved(QMouseEvent *eventMouseMoved)
{

    int x_click =  ui->widget->xAxis->pixelToCoord(eventMouseMoved->pos().x());
    double y_click =  ui->widget->yAxis->pixelToCoord(eventMouseMoved->pos().y());
    QString coord_click = "x = " + QString::number(x_click) + "  y = " + QString::number(y_click,'f',3);
    ui->statusBar->showMessage(coord_click);

}










void MainWindow::keyPressEvent(QKeyEvent *eventKeypress){

    qDebug() << "press key";
        int keyValue = eventKeypress->key();

        if(keyValue>47 && keyValue<57){
            cur_graph=keyValue-49; // При нажатии на 0-9 переключается tracer на соответствующий график
            qDebug() << "tracer on" << cur_graph << "graph";
        }else if(keyValue == Qt::Key_Delete){
            qDebug() << "#delete#";
            on_actionClearGraph_triggered();


        }else if (keyValue == Qt::Key_Space){
            qDebug() << "#enter#";
            on_actionMakeGraph_triggered();

        }else if (keyValue == Qt::Key_Escape){
            qDebug() << "#escape#";
            QApplication::quit();
        }




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

    uint8_t r_color=0;
    uint8_t g_color=0;
    uint8_t b_color=0;

    while(r_color + g_color + b_color <50){
        r_color = rand() %255;
        g_color = rand() %255;
        b_color = rand() %255;
    }
    qDebug()<<"Color of line: "<< r_color << g_color << b_color;
    ui->widget->graph(graph_n)->setPen(QColor(r_color, g_color, b_color, 255));//задаем цвет точки
    //ui->widget->graph(graph_n)->setLineStyle(QCPGraph::lsNone);//убираем линии
    ui->widget->graph(graph_n)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 3));

    ui->widget->replot();


    ui->statusBar->showMessage("Cоздана кривая №"+QString::number(graph_n+1));




    graph_n++;




}

void MainWindow::on_actionClearGraph_triggered()
{
    ui->statusBar->showMessage("CLEAR A GRAPH");
    ui->widget->clearGraphs();
    ui->widget->replot();
    graph_n=0;



}

void MainWindow::on_actionMakeAconnection_triggered()
{
    connectWidget connect_w;
    connect_w.setModal(true);
    connect_w.exec();

}




