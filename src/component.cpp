#include "component.h"

#include <QDebug>
#include <QStyleOptionGraphicsItem>

extern double mousex;
extern double mousey;

component::component()
{
    text = new QGraphicsSimpleTextItem(this);
    next = NULL;

    this->setFlags(QGraphicsItem::ItemIsSelectable);
}

void component::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
//    QPen pen;

//    if(option->state & QStyle::State_Selected){
//        pen.setColor(Qt::blue);
//        pen.setWidth(6);
//        qDebug()<<"selected"<<getIndex();
//    }
//    else{
//        pen.setBrush(Qt::black);
//        pen.setWidth(1);
//        qDebug()<<"de-selected"<<getIndex();
//    }

//    painter->setPen(pen);
//    painter->drawRect(-30,-30,60,60);
}

QPointF component::getPos()
{
    return this->scenePos();
}


void component::draw()
{
    QPen blackpen(Qt::black);
    QPen bluepen(Qt::blue);
    QPen greenpen(Qt::green);
    QPen dotpen(Qt::red);
    QPen dredpen(Qt::darkRed);
    dotpen.setStyle(Qt::DashLine);
    greenpen.setWidth(2.5);
    bluepen.setWidth(2.5);
    dotpen.setWidth(2.5);



    qreal x1 = -30, x2 = -30, x3 = +30 ,x4= +30;
    QPointF p1 = QPointF(x1,x2);
    QPointF p2 = QPointF(x3,x4);
    QRectF rect;
    rect.setTopLeft(p1);
    rect.setBottomRight(p2);
    rectangle = new QGraphicsRectItem(this);
    rectangle->setRect(rect);
    rectangle->setPen(blackpen);

    text->moveBy(-20,18);
    text->setFlags(QGraphicsItem::ItemIsMovable);
    //todo: restrict the area where this text item can be moved
    text->setParentItem(this);

}

void component::setMovable(bool movable)
{
    if(movable){
        this->parentItem()->setFlags(QGraphicsItem::ItemIsSelectable
                       |QGraphicsItem::ItemIsMovable);
    }
    else{
        this->parentItem()->setFlags(QGraphicsItem::ItemIsSelectable);

    }
}

QRectF component::boundingRect() const
{

    qreal x1 = mousex-35, x2 = mousey-35, x3 = mousex+35 ,x4= mousey+35;
    QPointF p1 = QPointF(x1,x2);
    QPointF p2 = QPointF(x3,x4);
    QRectF rect;
    rect.setTopLeft(p1);
    rect.setBottomRight(p2);

    return rect;
}

QVariant component::itemChange(QGraphicsItem::GraphicsItemChange change, const QVariant &value)
{
    return QGraphicsItem::itemChange(change,value);
}

