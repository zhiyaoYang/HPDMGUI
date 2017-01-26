#include "link.h"

link::link(component *fromComp, component *toComp)
{
    myComp1 = fromComp;
    myComp2 = toComp;

    line = new QGraphicsLineItem(this);
    line->setFlags(QGraphicsItem::ItemIsSelectable);

    trackComp();
}

void link::trackComp()
{

    double dY = myComp1->scenePos().y() - myComp2->scenePos().y();
    double dX = myComp1->scenePos().x() - myComp2->scenePos().x();

    line1->setLine(QLineF(myComp2->scenePos(),myComp1->scenePos()));


//    myArrow->setPos((myFromNode->scenePos().x()+myToNode->scenePos().x())/2,(myFromNode->scenePos().y()+myToNode->scenePos().y())/2);
//    if(dX>=0)
//        myArrow->setRotation(-180*acos(dY/sqrt(dX*dX+dY*dY))/3.14);
//    else
//        myArrow->setRotation(180*acos(dY/sqrt(dX*dX+dY*dY))/3.14);

}

void link::setLineColor()
{
    QPen myPen;
    myPen.setColor(Qt::blue);
    myPen.setWidth(3);
    line->setPen(myPen);
}

void link::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{

}
