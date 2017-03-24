#ifndef COMPONENT_H
#define COMPONENT_H

#include <QGraphicsItem>
#include <QPainter>
#include <QString>
#include <QSet>

#include "link.h"

using namespace std;

class link;

struct parameter{
    int index;
    QString name;
    double value;
    QString description;
    QString iORd;
};

struct variable{
    int index;
    QString name;
    double value;
    QString description;
    QString solvingSetting;
    QString enabled;
};



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
    void setTypeIndex(int i){typeIndex = i;}
    int getTypeIndex(){return typeIndex;}
    void setCompName(QString name){componentName = name;}
    QString getCompName(){return componentName;}
    void setCompDescription(QString des){description = des;}
    QString getCompDescription(){return description;}

    variable findVar(int i){
        foreach(variable v, myVar){
            if(v.index==i){
                return v;
            }
        }
    }

    component* next;
    QGraphicsSimpleTextItem * text;
    QSet<link *> myLinks;

    QList<parameter> myPar;
    QList<variable> myVar;

    QString equation;

public slots:



private:

    QRectF boundingRect() const;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    QGraphicsRectItem * rectangle;


    int index;
    int typeIndex;
    QString componentName;
    QString description;

};

#endif // COMPONENT_H
