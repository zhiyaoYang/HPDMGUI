#include "link.h"

#include <QDebug>
#include <QStyleOptionGraphicsItem>

link::link(component *fromComp, component *toComp)
{
    myComp1 = fromComp;
    myComp2 = toComp;

    myComp1->addLink(this);
    myComp2->addLink(this);

    line = new QGraphicsLineItem(this);
    line->setFlags(QGraphicsItem::ItemIsSelectable|QGraphicsItem::ItemClipsToShape);
    this->setZValue(3);

    trackComp();
}

void link::trackComp()
{

    double dY = myComp1->scenePos().y() - myComp2->scenePos().y();
    double dX = myComp1->scenePos().x() - myComp2->scenePos().x();

    line->setLine(QLineF(myComp2->scenePos(),myComp1->scenePos()));

//later for stream direction
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
    QPen pen;
    pen.setColor(Qt::blue);
    pen.setWidth(3);
    painter->setPen(pen);

    painter->drawLine(myComp1->getPos(),myComp2->getPos());

    qDebug()<<option->state;
}

QRectF link::boundingRect() const
{
    QRectF rect;
    rect.setTopLeft(myComp1->getPos());
    rect.setBottomRight(myComp2->getPos());

    return rect;
}

QPainterPath link::shape() const
{
    //not making a difference
    QPainterPath path;
    double width = 2.5, height = 2.5;
    QPolygon polygon;
    polygon << QPoint(0, 0);
    polygon << QPoint(5, 5);
    polygon << QPoint(width, height);
    polygon << QPoint(width - 5, height - 5);
    path.addPolygon(polygon);

    return path;
}
