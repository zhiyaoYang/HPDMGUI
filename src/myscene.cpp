/*myscene.cpp
 * [SorpSim v1.0 source code]
 * [developed by Zhiyao Yang and Dr. Ming Qu for ORNL]
 * [last updated: 10/12/15]
 * =======================================================================
 * custom class based on QGraphicsScene
 * object is created and setup in myView (QGraphicsView)
 * operations in the operating panel is handled via myScene including:
 * mouse press, double click on items
 * called by various classes in the project
 */



#include "myscene.h"
#include "mymainwindow.h"
#include "component.h"
#include "mycompdialog.h"
#include "link.h"
#include "mylinkdialog.h"

#include <QtGui>
#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QGraphicsTextItem>
#include <QObject>
#include <QGraphicsItem>
#include <QDebug>
#include <QDrag>
#include <QMimeData>
#include <QLabel>
#include <QMessageBox>
#include <QStatusBar>
#include <QToolBar>
#include <QApplication>


double mousex;
double mousey;

extern component* head;
extern component* dummy;
extern component* tempComponent;
extern myMainwindow * theMainWindow;

extern QString sceneAction;


myScene::myScene()
{
    mousex = 0;
    mousey = 0;
}

void myScene::drawComponent(component * comp)
{
    QPen pen(Qt::white);
    rect = this->addRect(mousex-30,mousey-30,+60,+60);
    rect->setFlags(QGraphicsItem::ItemIsMovable|QGraphicsItem::ItemIsSelectable);
    rect->setPen(pen);
    rect->setZValue(2);


    int tempIndex = head->getIndex();
    head->next = comp;
    head = comp;
    head->next = NULL;
    head->setIndex(tempIndex+1);

    head->draw();
    head->text->setText("<Component"+QString::number(head->getIndex())+">");
    head->setCompName("test component name");
    head->setCompDescription("test component description");


    head->moveBy(mousex,mousey);

    head->setParentItem(rect);

    head->setMovable(true);

}

void myScene::enableDrag(bool compDrag, bool textDrag)
{
    component* iterator = dummy;
    while(iterator->next!=NULL){
        iterator = iterator->next;
        iterator->setMovable(compDrag);
        if(compDrag){
            iterator->text->setFlags(QGraphicsItem::ItemIsFocusable);
        }
        else {
            iterator->text->setFlags(QGraphicsItem::ItemIsMovable);
        }
    }

}


void myScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    theView = dynamic_cast<myView*>(views().first());
    QGraphicsScene::mousePressEvent(event);

//    QList <QGraphicsItem *> items = this->selectedItems();

    if(sceneAction == "newComponent"){//for adding components
        mousex = event->scenePos().x();
        mousey = event->scenePos().y();
        drawComponent(tempComponent);
        //evoke component selection dialog
        //followed by component property dialog

        QApplication::restoreOverrideCursor();

        sceneAction = "";
    }

}

void myScene::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsScene::mouseDoubleClickEvent(event);
    QList <QGraphicsItem *> items = this->selectedItems();
    QMessageBox *mb;

    if(!items.isEmpty()){
        QGraphicsItem* tempItem = items.first();
        qDebug()<<tempItem->zValue()<<tempItem->scenePos()<<tempItem->flags();
        if(sceneAction == ""){//evoke property dialog
            if(tempItem->zValue()==2){//component
                component* comp = dynamic_cast<component*>(tempItem->childItems().first());
                qDebug()<<"linked?"<<!comp->myLinks.isEmpty();
                myCompDialog * compDialog = new myCompDialog(comp);
                compDialog->exec();
            }
            else if(tempItem->zValue()==0){//link
                link * myLink = dynamic_cast<link*>(tempItem->childItems().first());
                myLinkDialog *linkDialog = new myLinkDialog(myLink);
                linkDialog->exec();
            }

        }
        else if(sceneAction == "makeLink"){//select component to make link
            if(tempItem->zValue()==2){//component
                component* comp = dynamic_cast<component*>(tempItem->childItems().first());
                QApplication::restoreOverrideCursor();

                if(selectedComponent.isEmpty()){//fist component
                    selectedComponent.append(comp);
                    mb = new QMessageBox("Making Links",
                                         "Please double click the other component of the new link.",
                                         QMessageBox::Information,
                                         QMessageBox::Yes,
                                         QMessageBox::Cancel,
                                         QMessageBox::NoButton);
                    if(mb->exec() == QMessageBox::Cancel){
                        selectedComponent.clear();
                        sceneAction = "";
                    }
                    else{
                        QApplication::setOverrideCursor(QCursor(Qt::CrossCursor));
                    }
                }
                else if(selectedComponent.count()==1){//second component

                    component* tempComp = selectedComponent.first();
                    QSet<component*> linkedComps;
                    foreach(link* myLink, comp->myLinks){
                        linkedComps.insert(myLink->getComp1());
                        linkedComps.insert(myLink->getComp2());
                    }

                    if(comp==tempComp){//selected the same component
                        mb = new QMessageBox("Making Links",
                                             "Please select two different components to link.\nDo you want to select another component?",
                                             QMessageBox::Warning,
                                             QMessageBox::Yes,
                                             QMessageBox::Cancel,
                                             QMessageBox::NoButton);
                        if(mb->exec()== QMessageBox::Yes){
                            QApplication::setOverrideCursor(QCursor(Qt::CrossCursor));
                        }
                        else{
                            selectedComponent.clear();
                            sceneAction = "";
                        }
                    }
                    else if(linkedComps.contains(tempComp)){//existing link between the two components

                        mb = new QMessageBox("Making Links",
                                             "There is already a link between the selected components.\nDo you want to select another component?",
                                             QMessageBox::Warning,
                                             QMessageBox::Yes,
                                             QMessageBox::Cancel,
                                             QMessageBox::NoButton);
                        if(mb->exec()== QMessageBox::Yes){
                            QApplication::setOverrideCursor(QCursor(Qt::CrossCursor));
                        }
                        else{
                            selectedComponent.clear();
                            sceneAction = "";
                        }
                    }
                    else{//link two selected components
                        link* myLink = new link(comp,tempComp);
                        this->addItem(myLink);
                        qDebug()<<"evoke link dialog";
                        myLinkDialog * linkDialog = new myLinkDialog(myLink);
                        linkDialog->exec();

                        enableDrag(true);

                        QApplication::restoreOverrideCursor();


//http://stackoverflow.com/questions/8187807/itemchanged-never-called-on-qgraphicsitem
//http://doc.qt.io/qt-5/qtwidgets-graphicsview-dragdroprobot-example.html
                        selectedComponent.clear();
                        sceneAction = "";
                    }

                }
            }

        }


    }



}
