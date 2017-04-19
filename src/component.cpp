#include "component.h"

#include <QDebug>
#include <QStyleOptionGraphicsItem>


component::component()
{
    componentLabel = new QGraphicsSimpleTextItem(this);
    next = NULL;

    this->setFlags(QGraphicsItem::ItemSendsScenePositionChanges);

    this->setZValue(2);
}

//void component::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
//{
////    QPen pen;

////    if(option->state & QStyle::State_Selected){
////        pen.setColor(Qt::blue);
////        pen.setWidth(6);
////        qDebug()<<"selected"<<getIndex();
////    }
////    else{
////        pen.setBrush(Qt::black);
////        pen.setWidth(1);
////        qDebug()<<"de-selected"<<getIndex();
////    }

////    painter->setPen(pen);
////    painter->drawRect(-30,-30,60,60);
//}

QPointF component::getPos()
{
    return this->scenePos();
}


void component::draw()
{

    componentLabel->moveBy(15,55);
    componentLabel->setFlags(QGraphicsItem::ItemIsFocusable);
    //todo: restrict the area where this text item can be moved
    componentLabel->setParentItem(this);

}

void component::setMovable(bool movable)
{
    if(movable){
        this/*->parentItem()*/->setFlags(QGraphicsItem::ItemIsSelectable
                       |QGraphicsItem::ItemIsMovable
                                     |QGraphicsItem::ItemSendsScenePositionChanges);
    }
    else{
        this/*->parentItem()*/->setFlags(QGraphicsItem::ItemIsSelectable
                                     |QGraphicsItem::ItemSendsScenePositionChanges);

    }
}

//QRectF component::boundingRect() const
//{

//    double width = 35;

//    qreal x1 = -width, x2 = -width, x3 = +width ,x4= +width;
//    QPointF p1 = QPointF(x1,x2);
//    QPointF p2 = QPointF(x3,x4);
//    QRectF rect;
//    rect.setTopLeft(p1);
//    rect.setBottomRight(p2);

//    return rect;
//}

QVariant component::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemScenePositionHasChanged){
        if(!myLinks.isEmpty()){
            foreach(link* myLink,myLinks){
                myLink->trackComp();
            }
        }
    }
    return QGraphicsItem::itemChange(change,value);
}

