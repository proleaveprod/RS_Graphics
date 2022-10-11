#include "examplethreads.h"
#include <QDebug>

ExampleThreads::ExampleThreads(QString threadName) :
    name(threadName)
{

}

void ExampleThreads::run()
{
    for (int i = 0; i <= 100; i++ ) {
        qDebug() << name << " " << i;
    }
}
