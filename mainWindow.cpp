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

#define     MSG_LENGHT  4


float amps[MAX_N] ={};
int counter=0;

double xStart = 0; //Начало интервала, где рисуем график по оси Ox
double xEnd =  MAX_N-1; //Конец интервала, где рисуем график по оси Ox
double h = 1; //Шаг, с которым будем пробегать по оси Ox
uint8_t graph_n,cur_graph;

extern QSerialPort serial;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);

    //MainWindow::showMaximized();  // Разворачивание окна
    resize(400,300);
    connect(ui->widget,SIGNAL(mousePress(QMouseEvent*)),SLOT(clickedGraph(QMouseEvent*)));
    connect(ui->widget, SIGNAL(mouseMove(QMouseEvent*)),SLOT(mouseMoved(QMouseEvent*)));
    connect(&serial, SIGNAL(readyRead() ),this, SLOT( on_readSerial() ) );

    graphSetup();











}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SlotsignalConnected()
{
     connectWidget connect_wa;
    ui->statusBar->showMessage("Connected to :" + connect_wa.cur_com);

}

void MainWindow::clickedGraph(QMouseEvent *event)
{

    qDebug()<<"ACTION!";
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
        }else if(keyValue == Qt::Key_Tab){
            qDebug() << "#tab#";
            on_actionMakeAconnection_triggered();
        }




}


void MainWindow::on_actionMakeGraph_triggered()
{

     //Массивы координат точек

    //##############СОЗДАНИЕ ПРОИЗВОЛЬНОЙ СИНУСОИДЫ################################
//    int a = 10 + rand() % 1000;
//    qDebug()<<"A = "<< a;
//    double f = 1 + rand() % 100;
//    f= f/321213;
//    qDebug()<<"f = "<< f;
//    double fi = rand() % 360;
//    qDebug()<<"fi = "<< fi;
//    fi = fi*M_PI/180;
//    for(int i=0;i<MAX_N;i++)amps[i]=a*qSin(2*M_PI*f*i + fi);  // Вырисовка графика
//    for (int i= xStart;i<=xEnd;i+=h){x.push_back(i);y.push_back(amps[i]);} // Отправка в массив
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
    ui->widget->graph(graph_n)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));

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
    counter=0;

    for(int i=0;i<MAX_N;i++)amps[i]=0;

}

void MainWindow::on_actionMakeAconnection_triggered()
{
    connectWidget connect_w;
    connect_w.setModal(true);
    connect_w.exec();

}


void MainWindow::on_readSerial(){

        static uint8_t data_counter;
        static QByteArray rdata;
        QByteArray dataArray = serial.readAll();

        uint8_t lenght = dataArray.length();
        //qDebug()<<"lenght:"<<lenght;

        if(lenght<MSG_LENGHT){
            rdata=rdata+dataArray;
            //qDebug()<<"so, data:"<<rdata;
            data_counter+=lenght;
            //qDebug()<<"____counter:"<<data_counter;
        }else {
            rdata=dataArray;
            //qDebug()<<"FULLdata:"<<data;
            data_counter=MSG_LENGHT;
        }
        if(data_counter==MSG_LENGHT){
            //qDebug()<<"all!!!\n";
            //bool convertCheck=0;

            graphMaker(rdata);

            rdata="";
            data_counter=0;
        }else if(data_counter>8){
            qDebug()<<"@upper";
        }


/*
        //bool check;
        //int signal = data.toInt(&check);

//        if(!check)  ui->statusBar->showMessage("ERROR!!!");
//        else{
//            ui->statusBar->showMessage("DATA =" + data);
//            amps[counter] = signal;
//            qDebug()<<"amp "<< counter <<"="<<amps[counter];

//            x.push_back(counter);
//            y.push_back(amps[counter]);
//            counter++;
//            if(counter==MAX_N)counter=0;
//        }

//        ui->widget->graph(0)->setData(x, y);
//        ui->widget->replot();
*/
}

void MainWindow::graphMaker(QByteArray rec_data){

    memcpy(&amps[counter],rec_data.data(),4);
    qDebug()<<"amps["<<counter<<"] ="<<amps[counter];

    x.push_back(counter);
    y.push_back(amps[counter]);
    ui->widget->graph(graph_n)->setData(x, y);
    counter++;

    ui->widget->replot();


}


void MainWindow::graphSetup(){
    ui->widget->setBackground(QColor(GrayCLR)); // Настройка графика

    ui->widget->xAxis->setRange(0,MAX_N);
    ui->widget->yAxis->setRange(0, 300);
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

    tracer = new QCPItemTracer(ui->widget);           //Настройка трэйсера
        //tracer->setBrush(QBrush(Qt::red));
    tracer->setPen(QPen(QColor(YellowCLR), 1, Qt:: SolidLine));
    tracer->setStyle(QCPItemTracer::tsCrosshair);
    tracer->setSize(1.0);

    tracerLabel = new QCPItemText(ui->widget); // Настройка надписи трэйсера
    tracerLabel->setLayer("overlay");
    tracerLabel->setPadding(QMargins(10,10,10,10));
    tracerLabel->setPositionAlignment(Qt::AlignLeft | Qt::AlignBottom);
    tracerLabel->setText(" ");
    tracerLabel->setColor(QColor(YellowCLR));
    tracerLabel->position->setParentAnchor(tracer->position);

    //for (int i= xStart;i<=xEnd;i+=h){x.push_back(i);y.push_back(amps[i]);}
    ui->widget->addGraph();

    ui->widget->graph(graph_n)->setData(x, y);
    ui->widget->graph(graph_n)->setPen(QColor(OrangeCLR));//задаем цвет точки
    ui->widget->graph(graph_n)->setLineStyle(QCPGraph::lsNone);
    ui->widget->graph(graph_n)->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 2));

}

/* Алгоритмы
 *
 *
//    QScreen *screen = QGuiApplication::primaryScreen();   // Алгоритм получения размера окна
//    QRect screenGeometry = screen->geometry();
//    int screenHeight = screenGeometry.height();
//    int screenWidth = screenGeometry.width();
*/



