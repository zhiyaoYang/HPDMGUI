/*myview.cpp
 * [HPDMGUI v0.0 source code]
 * [developed by Zhiyao Yang and Dr. Bo Shen for ORNL]
 * [last updated: 4/4/17]
 *
 * custom class based on QGraphicsView to control the viewing of the operating panel
 * mainly handles zoom (scaling) actions and panning actions
 * called by mymainwindow.cpp
 */




#include "myview.h"
#include "mymainwindow.h"
#include "component.h"
#include <QWheelEvent>
#include <QDebug>
#include <math.h>
#include "myscene.h"
#include <QShortcut>

extern component * dummy;
extern myScene * theScene;

myView::myView(QWidget *parent)
{
    myScale = 1;
}

void myView::wheelEvent(QWheelEvent *event)
{
    if (event->delta() >0&&myScale<1.2)
    {
        this->scale(1.1,1.1);
        myScale = myScale*1.1;
    }
    if(event->delta()<0&&myScale>0.4)
    {
        this->scale(0.8,0.8);
        myScale = myScale*0.8;
    }
    setScale();

}

void myView::setScale()
{
    component *iterator = dummy;
    while(iterator->next!=NULL)
    {
        iterator = iterator->next;
        iterator->text->setScale(1/myScale);
    }
}
