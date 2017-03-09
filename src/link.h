#ifndef LINK_H
#define LINK_H

#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QPen>
#include <cmath>

#include "component.h"

class component;

class link : public QGraphicsItem
{
public:

    link(component * fromComp, component * toComp);

    component *getComp1(){return myComp1;}
    component *getComp2(){return myComp2;}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void trackComp();

private:

    component* myComp1;
    component* myComp2;
    QGraphicsLineItem * line;

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setLineColor();

};

#endif // LINK_H
