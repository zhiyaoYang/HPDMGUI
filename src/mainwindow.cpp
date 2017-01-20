/*
 * mainwindow.cpp
 * [HPDMGUI v0.0 source code]
 * [developed by Zhiyao Yang and Dr. Bo Shen for ORNL]
 * [last updated: 1/19/2017]
 * ================================================
 * The mainwindow is the major operating interface of HPDMGUI, the first window to show when the program
 * launches, and last when users exit the program.
 */

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "component.h"

#include <QDebug>

component * dummy;
component * head;
component * tempComponent;

bool isDrawComponent;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("HPDMGUI");

    view = new myView(this);
    scene = new myScene();
    scene ->setSceneRect(-10000, -10000, 20000, 20000);
    view->setScene(scene);
    view->setRenderHint(QPainter::TextAntialiasing);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


    setCentralWidget(view);
    view->show();

    head = new component();
    dummy = head;
    head->setIndex(0);


    isDrawComponent = false;

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionExit_triggered()
{
    exit(0);
}

void MainWindow::on_actionTest_triggered()
{
    //draw a component
    tempComponent = new component();
    QApplication::setOverrideCursor(QCursor(Qt::CrossCursor));
    isDrawComponent = true;
}

void MainWindow::on_actionDrag_Drop_toggled(bool arg1)
{
    component* iterator = dummy;
    while(iterator->next!=NULL){
        iterator = iterator->next;
        iterator->setMovable(arg1);
    }
}

void MainWindow::on_actionDebug_triggered()
{

}
