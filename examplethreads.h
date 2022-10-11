#ifndef EXAMPLETHREADS_H
#define EXAMPLETHREADS_H

#include <QThread>



class ExampleThreads : public QThread
{
public:
    explicit ExampleThreads(QString threadName);

    // Переопределяем метод run(), в котором будет
    // располагаться выполняемый код
    void run();
private:
    QString name;   // Имя потока
};

#endif // EXAMPLETHREADS_H
