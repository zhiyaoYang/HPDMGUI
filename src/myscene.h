#ifndef MYSCENE_H
#define MYSCENE_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include "myview.h"
#include "component.h"

QT_BEGIN_NAMESPACE

class QGraphicsSceneMouseEvent;

QT_END_NAMESPACE

class myScene :public  QGraphicsScene
{
public:
    myScene();
    QGraphicsSimpleTextItem * copcap;
    QGraphicsRectItem * copRect;

public slots:

    void drawComponent(component * comp);


protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);

private:

    QGraphicsRectItem * rect;
    myView* theView;


};

#endif // MYSCENE_H
