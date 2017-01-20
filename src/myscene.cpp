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
#include <QtGui>
#include <QGraphicsSceneMouseEvent>
#include "mainwindow.h"
#include <QPen>
#include <QGraphicsTextItem>
#include <QObject>
#include "component.h"

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

extern bool isDrawComponent;


myScene::myScene()
{
    mousex = 0;
    mousey = 0;
}

void myScene::drawComponent(component * comp)
{
    QPen pen(Qt::white);
    rect = this->addRect(mousex-30,mousey-30,+60,+60);
    rect->setFlags(QGraphicsItem::ItemIsMovable/*|QGraphicsItem::ItemIsSelectable*/);
    rect->setPen(pen);
    rect->setZValue(2);


    int tempIndex = head->getIndex();
    head->next = comp;
    head = comp;
    head->next = NULL;
    head->setIndex(tempIndex+1);

    head->draw();
    head->text->setText("<C"+QString::number(head->getIndex())+">");


    head->moveBy(mousex,mousey);

    head->setParentItem(rect);

}


void myScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

    theView = dynamic_cast<myView*>(views().first());
    QGraphicsScene::mousePressEvent(event);

//    QList <QGraphicsItem *> items = this->selectedItems();




    if(isDrawComponent){//for adding components
        mousex = event->scenePos().x();
        mousey = event->scenePos().y();
        drawComponent(tempComponent);

        QApplication::restoreOverrideCursor();

        isDrawComponent = false;
    }

}

