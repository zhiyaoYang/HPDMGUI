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
#include <QGraphicsItem>
#include <QMessageBox>


component * dummy;
component * head;
component * tempComponent;

QString sceneAction;

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

bool myMainwindow::openHPDM()
{
    QString name = QFileDialog::getOpenFileName(this,"Open a .hpdm file","./","HPDM files(*.hpdm)");
    if(name!=""){
        return loadHPDMFile(name);
    }
    else{
        return false;
    }
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
    QApplication::setOverrideCursor(QCursor(Qt::CrossCursor));//later move to component selection dialog
    sceneAction = "newComponent";
}

void myMainwindow::newLink()
{

    QMessageBox * mb= new QMessageBox("New Link",
                         "Double click to select a component to create a new link from it.",
                         QMessageBox::Information,
                         QMessageBox::Yes,
                         QMessageBox::Cancel,
                         QMessageBox::NoButton);

    int ret = mb->exec();
    if(ret == QMessageBox::Yes){

        QApplication::setOverrideCursor(QCursor(Qt::CrossCursor));
        sceneAction = "makeLink";

    }
    else{
        QApplication::restoreOverrideCursor();
        sceneAction = "";
    }

}

void myMainwindow::enableDrag(bool compDrag)
{
    scene->enableDrag(compDrag);
}

void myMainwindow::help()
{
    QStringList list;
    list<<"help"<<"test";
    fillDock(list);
}

void myMainwindow::about()
{
    QStringList list;
    list<<"about"<<"test";
    fillDock(list);
}

void myMainwindow::initialize()
{
    setWindowTitle(tr("HPDM-Win"));

    head = new component();
    dummy = head;
    head->setIndex(0);

    sceneAction = "";

    createActions();
    createMenus();
    createDockWindows();
    createStatusBar();

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

    openHPDM = new QAction(tr("&Load .hpdm File"),this);
//    loadHPDMAct->setShortcuts(QKeySequence::Open);//crl+L
    openHPDM->setStatusTip(tr("Load a .hpdm file"));
    connect(openHPDM,&QAction::triggered,this,&myMainwindow::openHPDM);

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

    newLinkAct = new QAction(tr("&New Link"),this);
    newLinkAct->setStatusTip(tr("Add a new link"));
    connect(newLinkAct,&QAction::triggered,this,&myMainwindow::newLink);

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
    editMenu->addAction(newLinkAct);
    editMenu->addSeparator();
    editMenu->addAction(enableDragAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(helpAct);
    helpMenu->addAction(aboutAct);
}

void myMainwindow::createDockWindows()
{
    sysPicDock = new QDockWidget(tr("System Diagram"),this);
    sysPicDock->setAllowedAreas(Qt::RightDockWidgetArea);

    QPixmap pic("System.png");
    pic = pic.scaled(500,500,Qt::KeepAspectRatio);
    sysPic = new QLabel(sysPicDock);
    sysPic->setPixmap(pic);
    sysPicDock->setWidget(sysPic);
    addDockWidget(Qt::RightDockWidgetArea,sysPicDock);


    compListDock = new QDockWidget(tr("Component List"),this);
    compListDock->setAllowedAreas(Qt::RightDockWidgetArea);
    introduction = new QListWidget(compListDock);
    introduction->addItems(QStringList()
                           <<"index"
                           <<"description"
                           <<"properties");
    compListDock->setWidget(introduction);
    addDockWidget(Qt::RightDockWidgetArea,compListDock);

    addDockWidget(Qt::RightDockWidgetArea,sysPicDock);
    addDockWidget(Qt::RightDockWidgetArea,compListDock);
}

void myMainwindow::createStatusBar()
{

}

bool myMainwindow::loadHPDMFile(QString name)
{
    setWindowTitle("HPDM-"+name);
    QFile ofile(name);
    if(!ofile.open(QIODevice::ReadOnly|QIODevice::Text)){
        reportError("Failed to open .hpdm file.");
        return false;
    }
    else{
        QTextStream stream(&ofile);
        QString line;
        QStringList comp;
        QStringList links;
        QStringList splitList;


    }
}

void myMainwindow::reportError(QString err)
{
    QMessageBox *eBox = new QMessageBox;
    eBox->setIcon(QMessageBox::Warning);
    eBox->setText(err);
    eBox->exec();
}
