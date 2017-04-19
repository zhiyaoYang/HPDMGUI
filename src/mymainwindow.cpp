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
#include <QString>

component * dummy;
component * head;
component * tempComponent;
myMainwindow * mWindow;

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
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->setMargin(5);
    layout->addWidget(view);
    widget->setLayout(layout);

    myToolBar = addToolBar(tr("top"));
    QList<QAction*> list;
    list.append(openHPDMAct);
    list.append(panAct);
    list.append(selectAct);
    list.append(zoomToFitAct);

    myToolBar->setEnabled(true);



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
        QStringList list = name.split("/");
        list.removeLast();
        caseDirectory = list.join("/");
        QPixmap pic(caseDirectory+"/System.png");
        pic = pic.scaled(400,400,Qt::KeepAspectRatio);
        sysPic = new QLabel(sysPicDock);
        sysPic->setPixmap(pic);
        sysPicDock->setWidget(sysPic);
        enableDock(true);

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
    tempComponent = new component;
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

void myMainwindow::enableDock(bool eDock)
{
    sysPicDock->setVisible(eDock);
    compListDock->setVisible(eDock);

}

void myMainwindow::enableLink(bool eLink)
{
    component * iterator = dummy;

    while(iterator->next!=NULL){
        iterator = iterator->next;

        if(!iterator->myLinks.isEmpty()){
            foreach(link* l,iterator->myLinks){
                l->setVisible(eLink);
            }
        }
    }
}

void myMainwindow::switchPan()
{
    view->setDragMode(QGraphicsView::ScrollHandDrag);
    selectAct->setChecked(false);
    panAct->setChecked(true);
    view->setInteractive(false);
}

void myMainwindow::switchSelect()
{
    view->setDragMode(QGraphicsView::NoDrag);
    selectAct->setChecked(true);
    panAct->setChecked(false);
    view->setInteractive(true);
}

void myMainwindow::zoomToFit()
{
    double xb=0, yb=0, xmax = 0, ymax = 0, xratio = 0, yratio = 0, ratio = 0;
    component * iterator = dummy;

    if(iterator->next!=NULL)
    {
        xmax = -fabs(iterator->next->scenePos().x());
        ymax = -fabs(iterator->next->scenePos().y());

        int count = 0;

        while(iterator->next!=NULL)
        {
            iterator = iterator->next;
            count ++;
            xb+=iterator->scenePos().x();
            yb+=iterator->scenePos().y();
            if(xmax<iterator->scenePos().x())
                xmax = iterator->scenePos().x();
            if(ymax<iterator->scenePos().y())
                ymax = iterator->scenePos().y();
        }

        xb = xb / count;
        yb = yb / count;

        xratio = view->size().width()/((2*(xmax-xb+100)+1)*view->getScale());
        yratio = view->size().height()/((2*(ymax-yb+100)+1)*view->getScale());

        ratio = xratio;

        if(ratio>yratio)
            ratio = yratio;

        if(ratio*view->getScale()<=2)
        {
            view->setNewScale(view->getScale()*ratio);
            view->scale(ratio,ratio);
            view->setScale();
        }
        else
        {
            view->scale(2/view->getScale(),2/view->getScale());
            view->setNewScale(2);
            view->setScale();
        }
        view->centerOn(xb,yb);
    }
}

void myMainwindow::help()
{
    QStringList list;
    list<<"help"<<"test";
    fillDock(list);

    component * iterator = dummy;
    while(iterator->next!=NULL){
        iterator = iterator->next;

        if(iterator->equation!=""){
            iterator->parentItem()->hide();
            foreach(link* l,iterator->myLinks){
                l->hide();
            }
        }
    }
}

void myMainwindow::about()
{
    QStringList list;
    list<<"about"<<"test";
    fillDock(list);

    component * iterator = dummy;
    while(iterator->next!=NULL){
        iterator = iterator->next;

        if(iterator->equation!=""){
            iterator->parentItem()->show();
            foreach(link* l,iterator->myLinks){
                l->show();
            }
        }
    }
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


    saveAct = new QAction(tr("&Save"),this);
    saveAct->setShortcuts(QKeySequence::Save);//crl+S
    saveAct->setStatusTip(tr("Save current case"));
    connect(saveAct,&QAction::triggered,this,&myMainwindow::save);

    openHPDMAct = new QAction(tr("&Load .hpdm File"),this);
//    loadHPDMAct->setShortcuts(QKeySequence::Open);//crl+L
    openHPDMAct->setStatusTip(tr("Load a .hpdm file"));
    connect(openHPDMAct,&QAction::triggered,this,&myMainwindow::openHPDM);

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
    enableDragAct->setStatusTip(tr("Toggle dragging"));
    enableDragAct->setCheckable(true);
    enableDragAct->setChecked(true);
    connect(enableDragAct,&QAction::toggled,this,&myMainwindow::enableDrag);

    enableDockAct = new QAction(tr("&Show/Hide Dock"),this);
    enableDockAct->setStatusTip(tr("Toggle show/hide dock"));
    enableDockAct->setCheckable(true);
    enableDockAct->setChecked(true);
    connect(enableDockAct,&QAction::toggled,this,&myMainwindow::enableDock);

    panAct = new QAction(tr("&Pan"),this);
    panAct->setStatusTip(tr("Pan mode to control the view"));
    panAct->setCheckable(true);
    panAct->setChecked(false);
    connect(panAct,&QAction::triggered,this,&myMainwindow::switchPan);


    enableLinkAct = new QAction(tr("&Show/Hide Links"),this);
    enableLinkAct->setStatusTip(tr(""));
    enableLinkAct->setCheckable(true);
    enableLinkAct->setChecked(false);
    connect(enableLinkAct,&QAction::triggered,this,&myMainwindow::enableLink);

    selectAct = new QAction(tr("&Select"),this);
    selectAct->setStatusTip(tr("Resume to select mode"));
    selectAct->setCheckable(true);
    selectAct->setChecked(true);
    connect(selectAct,&QAction::triggered,this,&myMainwindow::switchSelect);

    zoomToFitAct = new QAction(tr("&Center"),this);
    zoomToFitAct->setStatusTip(tr("Zoom to fit the entire system"));
    connect(zoomToFitAct,&QAction::triggered,this,&myMainwindow::zoomToFit);

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
    fileMenu->addAction(openHPDMAct);
    fileMenu->addAction(saveAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(newCompAct);
    editMenu->addAction(newLinkAct);
    editMenu->addSeparator();
    editMenu->addAction(enableLinkAct);
    editMenu->addAction(enableDragAct);
    editMenu->addAction(enableDockAct);
    editMenu->addAction(panAct);
    editMenu->addAction(selectAct);
    editMenu->addAction(zoomToFitAct);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(helpAct);
    helpMenu->addAction(aboutAct);
}

void myMainwindow::createDockWindows()
{
    sysPicDock = new QDockWidget(tr("System Diagram"),this);
    sysPicDock->setAllowedAreas(Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea,sysPicDock);
    sysPicDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    sysPicDock->setVisible(false);

    compListDock = new QDockWidget(tr("Component List"),this);
    compListDock->setAllowedAreas(Qt::RightDockWidgetArea);
    addDockWidget(Qt::RightDockWidgetArea,compListDock);
    compListDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
    compListDock->setVisible(false);



}

void myMainwindow::createStatusBar()
{

}

bool myMainwindow::loadHPDMFile(QString name)
{

    //save current?
    //clear scene
    setWindowTitle("HPDM-"+name);
    QFile ofile(name);
    if(!ofile.open(QIODevice::ReadOnly|QIODevice::Text)){
        reportError("Failed to open .hpdm file.");
        return false;
    }
    else{
        QTextStream stream(&ofile);
        QString line;
        QString tempStr;
        QStringList comp;
        QStringList splitList;
        component * loadComp;
        link* loadLink;

        line = stream.readLine();
        while(!line.contains("End of Components")){

            if(QString(line.at(0))== "C"){
                //add a new component
                loadComp = new component;

                comp = line.split("\t",QString::SkipEmptyParts);


                loadComp->setIndex(comp.at(1).toInt());//index
                loadComp->setTypeIndex(comp.at(2).toInt());//type index
                if(QString(comp.at(6))=="!"){
                    //fluid lines/aux components
                    loadComp->setCompName(comp.at(7));//name
                    tempStr = comp.at(8);
                    loadComp->setCompDescription(tempStr.replace("\"",""));//description (get rid of " ")
                }
                else{
                    loadComp->setCompName(comp.at(6));//name
                    tempStr = comp.at(7);
                    loadComp->setCompDescription(tempStr.replace("\"",""));//description (get rid of " ")
                }

                line = stream.readLine();
                while(QString(line.at(0))=="P"||QString(line.at(0))=="V"||QString(line.at(0))=="E"){
                    //insert variable/parameters into component
                    if(QString(line.at(0))=="P"){
                        parameter p;
                        splitList = line.split("\t",QString::SkipEmptyParts);
                        p.index = splitList.at(1).toInt();
                        p.iORd = splitList.at(2);
                        p.name = splitList.at(3);
                        p.value = splitList.at(5);
                        tempStr = splitList.at(7);
                        p.description = tempStr.replace("\"","");

                        loadComp->myPar.append(p);
                    }
                    else if(QString(line.at(0))=="V"){
                        variable v;
                        splitList = line.split("\t",QString::SkipEmptyParts);
                        v.index = splitList.at(1).toInt();
                        v.solvingSetting = splitList.at(2);
                        v.name = splitList.at(3);
                        v.enabled = splitList.at(5);
                        if(v.solvingSetting=="r"||v.solvingSetting=="o"){
                            v.value = "";
                            tempStr = splitList.at(7);
                            v.description = tempStr.replace("\"","");
                        }
                        else{
                            v.value = splitList.at(6);
                            tempStr = splitList.at(8);
                            v.description = tempStr.replace("\"","");
                        }

                        loadComp->myVar.append(v);

                    }
                    else if(QString(line.at(0))=="E"){
                        //later integrate with equation function?
                        loadComp->equation = line;
                    }
                    line = stream.readLine();
                }

                //add component into the system
                scene->drawComponent(loadComp,70*loadComp->getIndex(),0);
                if(true){
                    //loadComp->getTypeIndex()>0 equation components?
                }

            }
            else if(QString(line.at(0))=="!"){
                //skip the place-holder rows
                line = stream.readLine();
            }
        }

        while(QString(line.at(0))!="L"){
            line = stream.readLine();
        }
        while(QString(line.at(0))=="L"||QString(line.at(0))=="T"){
            //add links
            splitList = line.split("\t",QString::SkipEmptyParts);

            if(QString(line.at(0))=="L"){
                //stream or var connection
                int compNum1 = splitList.at(2).split(";").at(0).toInt();
                int num1 = splitList.at(2).split(";").at(1).toInt();
                int compNum2 = splitList.at(3).split(";").at(0).toInt();
                int num2 = splitList.at(3).split(";").at(1).toInt();

                component* comp1 = findComp(compNum1+1);
                component* comp2 = findComp(compNum2+1);

                //find out existing connections between two components
                bool connected = false;
                if(!comp1->myLinks.isEmpty()){
                    foreach(link* l,comp1->myLinks){
                        if(comp2==l->getOtherComponent(comp1)){
                            connected = true;
                            loadLink = l;
                        }
                    }
                }
                if(!connected){
                    loadLink = scene->addLink(comp1,comp2);
                }

                if(QString(splitList.at(1))=="V"){
                    //variable connection
                    varLink tempVar;
                    tempVar.fromComp = comp1;
                    tempVar.toComp = comp2;
                    tempVar.fromVarNum = num1;
                    tempVar.toVarNum = num2;
                    tempVar.description = QString(splitList.at(5));

                    loadLink->myVar.append(tempVar);
                }
                else{
                    //stream connection
                    streamLink tempStream;
                    tempStream.fromComp = comp1;
                    tempStream.toComp = comp2;
                    tempStream.fromPortNum = num1;
                    tempStream.toPortNum= num2;
                    tempStream.type = QString(splitList.at(1));
                    tempStream.description = QString(splitList.at(5));

                    loadLink->myStream.append(tempStream);

                }
            }
            else if(QString(line.at(0))=="T"){
                //successive connection
                int compNum1 = splitList.at(1).split(";").at(0).toInt();
                QString type1 = QString(splitList.at(1).split(";").at(1));
                int num1 = splitList.at(1).split(";").at(2).toInt();
                int compNum2 = splitList.at(2).split(";").at(0).toInt();
                QString type2 = QString(splitList.at(2).split(";").at(1));
                int num2 = splitList.at(2).split(";").at(2).toInt();

                component* comp1 = findComp(compNum1+1);
                component* comp2 = findComp(compNum2+1);

                //find out existing connections between two components
                bool connected = false;
                if(!comp1->myLinks.isEmpty()){
                    foreach(link* l,comp1->myLinks){
                        if(comp2==l->getOtherComponent(comp1)){
                            connected = true;
                            loadLink = l;
                        }
                    }
                }
                if(!connected){
                    loadLink = scene->addLink(comp1,comp2);
                }

                successiveLink tempSuc;
                tempSuc.fromComp = comp1;
                tempSuc.toComp = comp2;
                tempSuc.fromType = type1;
                tempSuc.toType = type2;
                tempSuc.fromNum = num1;
                tempSuc.toNum = num2;
                tempSuc.description = QString(splitList.at(4));

                loadLink->mySuccessive.append(tempSuc);

            }

            line = stream.readLine();
        }


        stream.flush();
        ofile.close();
    }

    enableLink(false);
    zoomToFit();

    return true;
}

void myMainwindow::reportError(QString err)
{
    QMessageBox *eBox = new QMessageBox;
    eBox->setIcon(QMessageBox::Warning);
    eBox->setText(err);
    eBox->exec();
}

component *myMainwindow::findComp(int i)
{
    component* iterator = dummy, * reComp = NULL;
    while(iterator->next!=NULL){
        iterator=iterator->next;
        if(iterator->getIndex()==i){
            reComp = iterator;
        }
    }

    return reComp;
}
