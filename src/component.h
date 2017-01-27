#ifndef COMPONENT_H
#define COMPONENT_H

#include <QGraphicsItem>
#include <QPainter>
#include <QString>
#include <QSet>

#include "link.h"

class link;

class component : public QGraphicsItem
{
public:
    component();


    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPointF getPos();
    void draw();
    void setMovable(bool movable);
    void addLink(link* mLink){myLinks.insert(mLink);}
    void removeLink(link* mLink){myLinks.remove(mLink);}
    void setIndex(int i){index = i;}
    int getIndex(){return index;}
    void setCompName(QString name){componentName = name;}
    QString getCompName(){return componentName;}
    void setCompDescription(QString des){description = des;}
    QString getCompDescription(){return description;}

    component* next;
    QGraphicsSimpleTextItem * text;
    QSet<link *> myLinks;

public slots:



private:

    QRectF boundingRect() const;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    QGraphicsRectItem * rectangle;


    int index;
    QString componentName;
    QString description;

};

#endif // COMPONENT_H
