#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "myscene.h"
#include "myview.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionExit_triggered();

    void on_actionTest_triggered();

    void on_actionDrag_Drop_toggled(bool arg1);

    void on_actionDebug_triggered();

private:
    Ui::MainWindow *ui;

    myScene *scene;
    myView *view;
};

#endif // MAINWINDOW_H
