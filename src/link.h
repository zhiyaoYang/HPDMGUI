#ifndef LINK_H
#define LINK_H

#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QPen>
#include <cmath>

#include "component.h"

class component;

struct streamLink{
    QString type;
    component* fromComp;
    component* toComp;
    int fromPortNum;
    int toPortNum;
    QString description;
};

struct varLink{
    component* fromComp;
    component* toComp;
    int fromVarNum;
    int toVarNum;
    QString description;
};

struct successiveLink{
    QString fromType;
    QString toType;
    component* fromComp;
    component* toComp;
    int fromNum;
    int toNum;
    QString description;
};

class link : public QGraphicsItem
{
public:

    link(component * fromComp, component * toComp);

    component *getComp1(){return myComp1;}
    component *getComp2(){return myComp2;}
    component* getOtherComponent(component* comp){
        return (comp==myComp1?myComp2:myComp1);
    }

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void trackComp();

    QList<streamLink> myStream;
    QList<varLink> myVar;
    QList<successiveLink> mySuccessive;

private:

    component* myComp1;
    component* myComp2;
    QGraphicsLineItem * line;

    QRectF boundingRect() const;
    QPainterPath shape() const;
    void setLineColor();

};

#endif // LINK_H
