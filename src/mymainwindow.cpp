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
#include "myparametricdialog.h"


#include <QDebug>
#include <QVBoxLayout>
#include <QMenuBar>
#include <QMenu>
#include <QContextMenuEvent>
#include <QApplication>
#include <QGraphicsItem>
#include <QMessageBox>
#include <QString>
#include <QProcess>

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

    caseDirectory = "./";

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
    int askSave = askToSave();
}

bool myMainwindow::openHPDM()
{
    if(dummy->next!=NULL){
        int askSave = askToSave();
        switch(askSave)
        {
        case 0://cancel
        {
            break;
        }
        case 1://save and proceed
        {
            QString sName = QFileDialog::getSaveFileName(this,"Save current case to file:",
                                                         caseDirectory,"HPDM files(*.hpdm)");
            bool noSave = false;
            while(sName==""&&(!noSave))
            {
                QMessageBox * mBox = new QMessageBox(this);
                mBox->addButton("Choose a directory",QMessageBox::YesRole);
                mBox->addButton("Don's save current case",QMessageBox::NoRole);
                mBox->setWindowTitle("Warning");
                mBox->setText("Please choose a directory to save the case!");
                mBox->setModal(true);
                mBox->exec();
                if(mBox->buttonRole(mBox->clickedButton())==QMessageBox::YesRole)
                    sName = QFileDialog::getSaveFileName(this,"Save current case to file:",
                                                         caseDirectory,"HPDM files(*.hpdm)");
                else if(mBox->buttonRole(mBox->clickedButton())==QMessageBox::NoRole)
                    noSave = true;
            }
            saveHPDMFile(sName);
            QString name = QFileDialog::getOpenFileName(this,"Open a .hpdm file",caseDirectory,"HPDM files(*.hpdm)");
            if(name!=""){
                loadHPDMFile(name);
            }
        }
        case 2:
        {
            QString name = QFileDialog::getOpenFileName(this,"Open a .hpdm file",caseDirectory,"HPDM files(*.hpdm)");
            if(name!=""){
                loadHPDMFile(name);
            }
        }
        }
    }
    else{
        QString name = QFileDialog::getOpenFileName(this,"Open a .hpdm file",caseDirectory,"HPDM files(*.hpdm)");
        if(name!=""){
            loadHPDMFile(name);
        }
    }

}

bool myMainwindow::saveHPDM()
{
    QString name = QFileDialog::getSaveFileName(this,"Save as a .hpdm file",
                                                caseDirectory,"HPDM files(*.hpdm)");
    if(name.split(".").last()=="hpdm"){
        return saveHPDMFile(name);
    }
    else{
        return false;
    }
}

void myMainwindow::exitProgram()
{
    QMessageBox * mb= new QMessageBox("Exit HPDM",
                         "Do you want to exit? All unsaved changes will be lost.",
                         QMessageBox::Information,
                         QMessageBox::Yes,
                         QMessageBox::Cancel,
                         QMessageBox::NoButton);

    if(mb->exec() == QMessageBox::Yes){
        exit(0);
    }
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
//    compListDock->setVisible(eDock);
    zoomToFit();

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

void myMainwindow::run()
{

    //running the calculation batch file
    QProcess p;
    p.setWorkingDirectory(caseDirectory);
    auto Command = QString("cmd.exe");
    auto Arguments = QStringList{
            QString("/C"),
            caseDirectory+"/RunHPDMFlex.bat"
    };

    p.start(Command,Arguments);
    p.waitForFinished(-1);

    if(p.exitCode()==0){
        QMessageBox::information(this,
                                 "Done",
                                 "Calculation finished.",
                                 QMessageBox::Ok);

        //check SysErrLog for further information
    }
    else{
        QMessageBox::information(this,
                                 "Error",
                                 "Failed to run the calculation batch file.",
                                 QMessageBox::Ok);

    }
}

void myMainwindow::paraRunSetup()
{
    myParametricDialog *pDialog = new myParametricDialog(caseDirectory);
    pDialog->exec();
}

void myMainwindow::help()
{
    QStringList list;
    list<<"help"<<"test";
    fillDock(list);

    component * iterator = dummy;
    while(iterator->next!=NULL){
        iterator = iterator->next;

        if(!iterator->myEqn.isEmpty()){
            iterator->parentItem()->hide();
            foreach(link* l,iterator->myLinks){
                l->hide();
            }
        }
    }
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

    sceneAction = "";

    createActions();
    createMenus();
    createDockWindows();
    createStatusBar();

}

void myMainwindow::createActions()
{
    newAct = new QAction(tr("&New"),this);
    newAct->setStatusTip(tr("Create a new case"));
    connect(newAct,&QAction::triggered,this,&myMainwindow::newFile);

    openHPDMAct = new QAction(tr("&Load .hpdm case file"),this);
    openHPDMAct->setStatusTip(tr("Load an existing case in .hpdm file format"));
    connect(openHPDMAct,&QAction::triggered,this,&myMainwindow::openHPDM);

    saveHPDMAct = new QAction(tr("&Save .hpdm case file"),this);
    saveHPDMAct->setStatusTip(tr("Save current case in .hpdm file format"));
    connect(saveHPDMAct,&QAction::triggered,this,&myMainwindow::saveHPDM);

    exitAct = new QAction(tr("&Exit"),this);
    exitAct->setStatusTip(tr("Exit HPDM gracefully"));
    connect(exitAct,&QAction::triggered,this,&myMainwindow::exitProgram);

    newCompAct = new QAction(tr("&New Component"),this);
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

    runAct = new QAction(tr("&Run"),this);
    runAct->setStatusTip(tr("Run calculation"));
    connect(runAct,&QAction::triggered,this,&myMainwindow::run);

    paraRunAct = new QAction(tr("&Parametric"),this);
    paraRunAct->setStatusTip(tr("Run parametric calculation"));
    connect(paraRunAct,&QAction::triggered,this,&myMainwindow::paraRunSetup);

    helpAct = new QAction(tr("&Help"),this);
    helpAct->setStatusTip(tr("Open HPDM documentation"));
    connect(helpAct,&QAction::triggered,this,&myMainwindow::help);

    aboutAct = new QAction(tr("&About"),this);
    aboutAct->setStatusTip(tr("View information about HPDM"));
    connect(aboutAct,&QAction::triggered,this,&myMainwindow::about);

}

void myMainwindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
//    fileMenu->addAction(newAct);
//    fileMenu->addAction(saveXMLAct);
//    fileMenu->addAction(openXMLAct);
    fileMenu->addAction(saveHPDMAct);
    fileMenu->addAction(openHPDMAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(runAct);
    editMenu->addAction(paraRunAct);
    editMenu->addSeparator();
//    editMenu->addAction(newCompAct);
    editMenu->addAction(newLinkAct);



    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(enableLinkAct);
    viewMenu->addAction(enableDragAct);
    viewMenu->addAction(enableDockAct);
    viewMenu->addSeparator();
    viewMenu->addAction(panAct);
    viewMenu->addAction(selectAct);
    viewMenu->addAction(zoomToFitAct);

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

//    compListDock = new QDockWidget(tr("Component List"),this);
//    compListDock->setAllowedAreas(Qt::RightDockWidgetArea);
//    addDockWidget(Qt::RightDockWidgetArea,compListDock);
//    compListDock->setFeatures(QDockWidget::NoDockWidgetFeatures);
//    compListDock->setVisible(false);

}

void myMainwindow::createStatusBar()
{

}

bool myMainwindow::loadHPDMFile(QString name)
{
    qDebug()<<"clear scene!";

    clearScene();

    qDebug()<<"scene cleared";

    QStringList list = name.split("/");
    list.removeLast();
    caseDirectory = list.join("/");
    QPixmap pic(caseDirectory+"/System.png");
    pic = pic.scaled(400,400,Qt::KeepAspectRatio);
    sysPic = new QLabel(sysPicDock);
    sysPic->setPixmap(pic);
    sysPicDock->setWidget(sysPic);
    enableDock(true);
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
        double xCoord = 0, yCoord = 0;
        bool hasCord = false;

        batchRunLines.clear();

        while(!line.contains("C\t0")){
            line = stream.readLine();
        }
        while(!line.contains("End of Components")){

            if(QString(line.at(0))== "C"){
                //add a new component
                loadComp = new component;
                hasCord = false;

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

                while(QString(line.at(0))=="P"||QString(line.at(0))=="V"||QString(line.at(0))=="E"||QString(line.at(0))=="X"){
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
                        if(splitList.at(6)=="!"){
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
                        equation eqn;
                        splitList = line.split("\t",QString::SkipEmptyParts);
                        int count = splitList.count();
                        for(int p = 0; p < count - 4;p++){
                            eqn.eqnString.append(splitList.at(1+p)+"\t");
                        }
                        eqn.eqnString.append(splitList.at(count-3));
                        eqn.description = splitList.last();
                        loadComp->myEqn.append(eqn);
                    }
                    else if(QString(line.at(0))=="X"){
                        splitList = line.split("\t",QString::SkipEmptyParts);
                        xCoord = splitList.at(1).toDouble();
                        yCoord = splitList.at(2).toDouble();
                        hasCord = true;
                    }
                    line = stream.readLine();
                }

                //add component into the system
                if(!hasCord){
                    xCoord = 70*loadComp->getIndex();
                    yCoord = 0;
                }
                scene->drawComponent(loadComp,xCoord,yCoord);
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
        while(!stream.atEnd()){
            line = stream.readLine();
            if(QString(line.at(0))!="!"){
                batchRunLines.append(QString(line));
            }
        }


        stream.flush();
        ofile.close();
    }

    enableLink(false);
    zoomToFit();

    return true;
}

bool myMainwindow::saveHPDMFile(QString name)
{
    QFile file(name);


    if(file.open(QIODevice::WriteOnly|QIODevice::Text)){
        QTextStream stream(&file);

        QString heads, components, linkages, tails;
        heads = "! Comment line to describe the system - to be inputted o	i	g	r	t	y	n";

        component *iter = NULL;
        QSet<link*> includedLinks;
        QList<streamLink> strLinks;
        QList<varLink> varLinks;
        QList<successiveLink> sucLinks;
        iter = dummy;
        while(iter->next!=NULL){
            iter = iter->next;

            if(iter->getTypeIndex()!=38&&iter->getTypeIndex()!=14){
                components.append("\nC\t"+QString::number(iter->getIndex()-1)
                                  +"\t"+QString::number(iter->getTypeIndex())
                                  +"\tn\t10001\t\t\t\t!\t"+iter->getCompName()
                                  +"\t\""+iter->getCompDescription()+"\"");
            }
            else{
                //for refrigerant line
                components.append("\nC\t"+QString::number(iter->getIndex()-1)
                                  +"\t"+QString::number(iter->getTypeIndex())
                                  +"\tn\t10001\t0\t\t\t!\t"+iter->getCompName()
                                  +"\t\""+iter->getCompDescription()+"\"");
            }

            for(int i = 0; i < iter->myPar.count();i++){
                parameter par = iter->myPar.at(i);
                components.append("\nP\t"+QString::number(par.index)+"\t"
                                  +par.iORd+"\t"+par.name+"\t0\t\t"+par.value
                                  +"\t\t\t!\t\""+par.description+"\"");
            }

            if(!iter->myEqn.isEmpty()){

                variable var = iter->myVar.first();
                components.append("\nV\t0\t"
                                  +var.solvingSetting+"\t"+var.name+"\t0\t"
                                  +var.enabled+"\t"+var.value+"\t\t\t!\t\""
                                  +var.description+"\"");

                equation eqn = iter->myEqn.first();
                components.append("\nE\t"+eqn.eqnString+"\t\t\t\t\t!\t"
                                  +eqn.description);
            }
            else{
                for(int i = 0; i < iter->myVar.count();i++){
                    variable var = iter->myVar.at(i);
                    components.append("\nV\t"+QString::number(var.index)+"\t"
                                      +var.solvingSetting+"\t"+var.name+"\t0\t"
                                      +var.enabled+"\t"+var.value+"\t\t\t!\t\""
                                      +var.description+"\"");
                }
            }
            components.append("\nX\t"+QString::number(iter->scenePos().x())
                              +"\t"+QString::number(iter->scenePos().y()));

            foreach(link* l, iter->myLinks){
                if(!includedLinks.contains(l)){
                    foreach(streamLink strL, l->myStream){
                        strLinks.append(strL);
                    }
                    foreach(varLink varL, l->myVar){
                        varLinks.append(varL);
                    }
                    foreach(successiveLink sucL, l->mySuccessive){
                        sucLinks.append(sucL);
                    }
                    includedLinks.insert(l);
                }
            }

        }

        components.append("\n{\tEnd of Components\t}\t\t\t\t\t\t\t!"
                          "\tend of components and equations\n"
                          "!\t After this row; input variable linkages; starting with \'L\'");

        for(int i = 0; i < strLinks.count();i++){
            streamLink strL = strLinks.at(i);
            linkages.append("\nL\t"+strL.type+"\t"
                            +QString::number(strL.fromComp->getIndex()-1)+";"
                            +QString::number(strL.fromPortNum)+"\t"
                            +QString::number(strL.toComp->getIndex()-1)+";"
                            +QString::number(strL.toPortNum)
                            +"\t!\t"+strL.description);
        }
        for(int i = 0; i < varLinks.count();i++){
            varLink varL = varLinks.at(i);
            linkages.append("\nL\tV\t"
                            +QString::number(varL.fromComp->getIndex()-1)+";"
                            +QString::number(varL.fromVarNum)+"\t"
                            +QString::number(varL.toComp->getIndex()-1)+";"
                            +QString::number(varL.toVarNum)
                            +"\t!\t"+varL.description);
        }
        for(int i = 0; i < sucLinks.count();i++){
            successiveLink sucL = sucLinks.at(i);
            linkages.append("\nT\t"+
                            QString::number(sucL.fromComp->getIndex()-1)+";"
                            +sucL.fromType+";"+QString::number(sucL.fromNum)+"\t"
                            +QString::number(sucL.toComp->getIndex()-1)+";"
                            +sucL.toType+";"+QString::number(sucL.toNum)+"\t!\t"
                            +sucL.description);
        }

        linkages.append("\n!\t After this row, input batch run inputs, starting with \'B\'");
        foreach(QString l, batchRunLines){
            tails.append("\n"+l);
        }

        stream<<heads<<components<<linkages<<tails;
        stream.flush();
        file.close();
    }

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

void myMainwindow::closeEvent()
{
    exitProgram();
}

int myMainwindow::askToSave()
{
    QMessageBox* askSaveBox = new QMessageBox(this);
    askSaveBox->addButton(QMessageBox::Ok);
    askSaveBox->addButton(QMessageBox::No);
    askSaveBox->addButton(QMessageBox::Cancel);
    askSaveBox->setWindowTitle("Please select...");
    askSaveBox->setText("Save current case?");
    askSaveBox->exec();
    if(askSaveBox->result()==QMessageBox::Ok)
    {
        //save current and proceed
        return 1;
    }
    else if(askSaveBox->result()==QMessageBox::No)
    {
        //discard current and proceed
        return 2;
    }
    else
    {
        //cancel the action
        return 0;
    }
}

void myMainwindow::clearScene()
{
    qDebug()<<"cs 1";

    enableDock(false);
    setWindowTitle("HPDM");

    qDebug()<<"cs 2";

    component* iter = dummy->next, *delComp = NULL;

    while(iter!=NULL && iter->next!=NULL){

        delComp = iter;
        iter = iter->next;

        qDebug()<<"cs 3.1"<<delComp->myLinks.count();

//        if(!delComp->myLinks.isEmpty()){
//            foreach(link* l, delComp->myLinks){
//                delete l;
//            }
//        }

        qDebug()<<"cs 3.2";

        delete delComp;
    }
}
