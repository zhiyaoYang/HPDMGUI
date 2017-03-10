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
#include <QList>

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
    void enableDrag(bool compDrag, bool textDrag = false);


protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

private:

    QGraphicsRectItem * rect;
    myView* theView;
    QList<component*> selectedComponent;

};

#endif // MYSCENE_H
