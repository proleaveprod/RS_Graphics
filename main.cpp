#include "mainWindow.h"
#include <QApplication>
#include <cstdlib>
#include "connectwidget.h"
#include "examplethreads.h"




int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    MainWindow main_w;
    main_w.show();


    ExampleThreads threadA("thread1");
    ExampleThreads threadC("thread2");

    ExampleThreads threadB("thread3");
    threadA.start();    // Запускаем потоки
    threadB.start();    // и наблюдаем их параллельную работу
    threadC.start();    // в выводе qDebug




    return a.exec();
}
