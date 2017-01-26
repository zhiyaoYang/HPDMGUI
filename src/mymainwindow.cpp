/*
 * mymainwindow.cpp
 * [HPDMGUI v0.0 source code]
 * [developed by Zhiyao Yang and Dr. Bo Shen for ORNL]
 * [last updated: 1/26/2017]
 * ================================================
 * The mainwindow is the major operating interface of HPDMGUI, the first window to show when the program
 * launches, and last when users exit the program.
 */

#include "mymainwindow.h"
#include "component.h"


#include <QDebug>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QContextMenuEvent>
#include <QApplication>


component * dummy;
component * head;
component * tempComponent;

bool isDrawComponent;

myMainwindow::myMainwindow(QWidget *parent) :
    QMainWindow(parent)
{

    QWidget * widget = new QWidget;
    setCentralWidget(widget);

    view = new myView(this);
    scene = new myScene();
    scene ->setSceneRect(-10000, -10000, 20000, 20000);
    view->setScene(scene);
    view->setRenderHint(QPainter::TextAntialiasing);
//    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
//    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);



    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(5);
    layout->addWidget(view);
    widget->setLayout(layout);

    initialize();
}

#ifndef QT_NO_CONTEXTMENU
void myMainwindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(newCompAct);
    menu.exec(event->globalPos());
}
#endif // QT_NO_CONTEXTMENU

void myMainwindow::newFile()
{

}

void myMainwindow::open()
{

}

void myMainwindow::save()
{

}

void myMainwindow::exitProgram()
{
    exit(0);
}

void myMainwindow::newComponent()
{
    //draw a component
    tempComponent = new component();
    QApplication::setOverrideCursor(QCursor(Qt::CrossCursor));
    isDrawComponent = true;
}

void myMainwindow::enableDrag(bool arg1)
{
    component* iterator = dummy;
    while(iterator->next!=NULL){
        iterator = iterator->next;
        iterator->setMovable(arg1);
    }
}

void myMainwindow::help()
{

}

void myMainwindow::about()
{

}

void myMainwindow::initialize()
{
    setWindowTitle(tr("HPDM-Win"));

    head = new component();
    dummy = head;
    head->setIndex(0);

    isDrawComponent = false;

    createActions();
    createMenus();

}

void myMainwindow::createActions()
{
    newAct = new QAction(tr("&New"),this);
//    newAct->setShortcuts(QKeySequence::New);//crl+N
    newAct->setStatusTip(tr("Create a new case"));
    connect(newAct,&QAction::triggered,this,&myMainwindow::newFile);

    openAct = new QAction(tr("&Open"),this);
//    openAct->setShortcuts(QKeySequence::Open);//crl+O
    openAct->setStatusTip(tr("Open an existing case"));
    connect(openAct,&QAction::triggered,this,&myMainwindow::open);

    saveAct = new QAction(tr("&Save"),this);
    saveAct->setShortcuts(QKeySequence::Save);//crl+S
    saveAct->setStatusTip(tr("Save current case"));
    connect(saveAct,&QAction::triggered,this,&myMainwindow::save);

    exitAct = new QAction(tr("&Exit"),this);
    exitAct->setStatusTip(tr("Exit HPDM gracefully"));
    connect(exitAct,&QAction::triggered,this,&myMainwindow::exitProgram);

    newCompAct = new QAction(tr("&New Component"),this);
//    newCompAct->setShortcuts(QKeySequence::New);
    newCompAct->setStatusTip(tr("Add a new component"));
    connect(newCompAct,&QAction::triggered,this,&myMainwindow::newComponent);

    enableDragAct = new QAction(tr("&Enable Dragging"),this);
//    enableDragAct->setShortcuts(QKeySequence::New);
    enableDragAct->setStatusTip(tr("Toggle dragging"));
    enableDragAct->setCheckable(true);
    enableDragAct->setChecked(true);
    connect(enableDragAct,&QAction::toggled,this,&myMainwindow::enableDrag);

    helpAct = new QAction(tr("&Help"),this);
//    helpAct->setShortcuts(QKeySequence::New);
    helpAct->setStatusTip(tr("Open HPDM documentation"));
    connect(helpAct,&QAction::triggered,this,&myMainwindow::help);

    aboutAct = new QAction(tr("&About"),this);
//    aboutAct->setShortcuts(QKeySequence::New);
    aboutAct->setStatusTip(tr("View information about HPDM"));
    connect(aboutAct,&QAction::triggered,this,&myMainwindow::about);

}

void myMainwindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(newCompAct);
    editMenu->addSeparator();
    editMenu->addAction(enableDragAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(helpAct);
    helpMenu->addAction(aboutAct);
}
