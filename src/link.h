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

    component *comp1() const {return myComp1;}
    component *comp2() const {return myComp2;}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

private:

    component* myComp1;
    component* myComp2;
    QGraphicsLineItem * line;

    void trackComp();
    void setLineColor();

};

#endif // LINK_H
