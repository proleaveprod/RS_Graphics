#include "mainWindow.h"
#include <QApplication>
#include <cstdlib>
#include "connectwidget.h"


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

    MainWindow main_w;
    main_w.show();

    return a.exec();
}
