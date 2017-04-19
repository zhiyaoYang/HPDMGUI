#include "component.h"

#include <QDebug>
#include <QStyleOptionGraphicsItem>


component::component()
{
    componentLabel = new QGraphicsSimpleTextItem(this);
    next = NULL;

    this->setFlags(QGraphicsItem::ItemSendsScenePositionChanges);

    this->setZValue(2);

    setOffset(-20,-20);
}


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
        this->setFlags(QGraphicsItem::ItemIsSelectable
                       |QGraphicsItem::ItemIsMovable
                                     |QGraphicsItem::ItemSendsScenePositionChanges);
    }
    else{
        this->setFlags(QGraphicsItem::ItemIsSelectable
                                     |QGraphicsItem::ItemSendsScenePositionChanges);

    }
}


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

