#ifndef COMPONENT_H
#define COMPONENT_H

#include <QGraphicsItem>
#include <QPainter>

class component : public QGraphicsItem
{
public:
    component();


    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option, QWidget *widget);
    QPointF getPos();
    void draw();
    void setMovable(bool movable);
    void setIndex(int i);
    int getIndex();

    component* next;
    QString componentName;
    QString description;
    QGraphicsSimpleTextItem * text;

public slots:



private:

    QRectF boundingRect() const;
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    QGraphicsRectItem * rectangle;
    int index;

};

#endif // COMPONENT_H
