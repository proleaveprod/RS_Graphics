#ifndef CONNECTWIDGET_H
#define CONNECTWIDGET_H

#include <QDialog>
#include <mainWindow.h>

namespace Ui {
class connectWidget;
}

class connectWidget : public QDialog
{
    Q_OBJECT

public:
    explicit connectWidget(QWidget *parent = nullptr);
    ~connectWidget();

private slots:
    void on_connectButton_clicked();

    void on_updateButton_clicked();

private:
    Ui::connectWidget *ui;
};

#endif // CONNECTWIDGET_H
