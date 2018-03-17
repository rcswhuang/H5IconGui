﻿#include "hiconrectitem.h"
#include "hrectangle.h"
#include <qmath.h>
//#include <QObject>
#include <QRectF>
#include <QPainterPath>
#include <QPointF>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
HIconRectItem::HIconRectItem(HIconGraphicsItem *parent)
    :HIconGraphicsItem(parent)
{

}

//ok
HIconRectItem::HIconRectItem(const QRectF &rectF, HIconGraphicsItem *parent)
    :HIconGraphicsItem(parent),rectF(rectF)
{
    setFlag(QGraphicsItem::ItemIsMovable,true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);
    setFlag(QGraphicsItem::ItemIsFocusable,true);
    pRectObj = NULL;
}

QRectF HIconRectItem::boundingRect() const
{ 
    return pRectObj->boundingRect();
}

bool HIconRectItem::contains(const QPointF &point) const
{
    return pRectObj->contains(point);
}

void HIconRectItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    pRectObj->paint(painter);
}

QPainterPath HIconRectItem::shape() const
{
    return pRectObj->shape();
}

int HIconRectItem::type() const
{
    return enumRectangle;
}

//ok
void HIconRectItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pointStart = event->scenePos();
    pointLocation = pointInRect(pointStart);
    HIconGraphicsItem::mousePressEvent(event);
}

//ok
void HIconRectItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qreal fRotateAngle = pRectObj->getRotateAngle();
    QTransform transform;
    //transform.translate(rect().center().x(),rect().center().y());
    transform.rotate(-fRotateAngle);
    QPointF pt = transform.map(event->scenePos()) - transform.map(pointStart);
    transform.rotate(fRotateAngle);

    QPointF pt1 = event->scenePos() - pointStart;
    qreal deltaX =pt.x();
    qreal deltaY = pt.y();
    pointStart = event->scenePos();
    bool bShift = false;
    if(event->modifiers() == Qt::ShiftModifier)
        bShift = true;
    if(pointLocation == 1)
    {
        QRectF rectNew;
        rectNew.setTopLeft(QPointF(rect().left() + deltaX,rect().top() + deltaY));
        rectNew.setBottomRight(rect().bottomRight());
        setRect(rectNew.normalized());
    }
    else if(pointLocation == 2)
    {
        QRectF rectNew;
        rectNew.setTopRight(QPointF(rect().right() + deltaX,rect().top() + deltaY));
        rectNew.setBottomLeft(rect().bottomLeft());
        setRect(rectNew.normalized());
    }
    else if(pointLocation == 3)
    {
        QRectF rectNew;
        rectNew.setBottomLeft(QPointF(rect().left() + deltaX,rect().bottom() + deltaY));
        rectNew.setTopRight(rect().topRight());
        setRect(rectNew.normalized());
    }
    else if(pointLocation == 4)
    {
        QRectF rectNew;
        rectNew.setBottomRight(QPointF(rect().right() + pt.x(),rect().bottom() + pt.y()));
        rectNew.setTopLeft(rect().topLeft());
        setRect(rectNew.normalized());
    }
    else
    {
        pRectObj->moveBy(pt.x(),pt.y());
        QRectF recttemp = pRectObj->getObjRect();
        setRect(recttemp.normalized());
        //HIconGraphicsItem::mouseMoveEvent(event);
    }
}

//ok
void HIconRectItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    HIconGraphicsItem::mouseReleaseEvent(event);
}

//ok
void HIconRectItem::keyPressEvent(QKeyEvent *event)
{
    int nStep = 5;
    if(event->modifiers() == Qt::ShiftModifier)
    {
        nStep = 1;
    }
    int ndx = 0;
    int ndy = 0;
    switch(event->key())
    {
    case Qt::Key_Up:
    {
        ndx = 0;
        ndy = -nStep;
        break;
    }
    case Qt::Key_Down:
    {
        ndx = 0;
        ndy = nStep;
        break;
    }
    case Qt::Key_Left:
    {
        ndx = -nStep;
        ndy = 0;
        break;
    }
    case Qt::Key_Right:
    {
        ndx = nStep;
        ndy = 0;
        break;
    }
    }
    if(ndx == 0 && ndy == 0)
        return;
    QRectF newRect = rect().adjusted(ndx,ndy,ndx,ndy);
    setRect(newRect);
}

//no
void HIconRectItem::setRect(const QRectF& rect)
{
    if(rect == rectF) return;
    prepareGeometryChange();
    rectF = rect;
    refreshBaseObj(rect);
    update();
}

QRectF HIconRectItem::rect() const
{
    return rectF;
}

void HIconRectItem::setItemObj(HBaseObj *pObj)
{
    pRectObj = (HRectangle*)pObj;
    pRectObj->setIconGraphicsItem(this);
}

HBaseObj* HIconRectItem::getItemObj()
{
    if(pRectObj)
        return pRectObj;
    return NULL;
}

//ok
void HIconRectItem::moveItemBy(qreal dx, qreal dy)
{
    QRectF newRectF;
    newRectF = rect().translated(dx,dy);
    setRect(newRectF);
}

//ok
void HIconRectItem::resizeItem(const QPolygonF& polygonF)
{
    if(polygonF.size() != 4)
        return;
    //topleft bottomright
    QRectF newRectF(polygonF.at(0),polygonF.at(3));
    setRect(newRectF);
}

void HIconRectItem::refreshBaseObj(const QRectF& rect)
{
    pRectObj->setObjRect(rect);
    QPointF p = rect.center();
    pRectObj->setOX(p.x());
    pRectObj->setOY(p.y());
    pRectObj->setModify(true);
}

//ok
void HIconRectItem::setItemCursor(int location)
{
    if(location == 1 || location == 4)
        setCursor(QCursor(Qt::SizeFDiagCursor));
    else if(location == 2 || location == 3)
        setCursor(QCursor(Qt::SizeBDiagCursor));
    else
        setCursor(QCursor(Qt::ArrowCursor));
}

//ok
ushort HIconRectItem::pointInRect(QPointF& point)
{
    qreal halfpw = 14.00;
    QRectF rect1,rect2,rect3,rect4;
    rect1.setSize(QSizeF(halfpw,halfpw));
    rect1.moveCenter(rect().topLeft());
    rect2.setSize(QSizeF(halfpw,halfpw));
    rect2.moveCenter(rect().topRight());
    rect3.setSize(QSizeF(halfpw,halfpw));
    rect3.moveCenter(rect().bottomLeft());
    rect4.setSize(QSizeF(halfpw,halfpw));
    rect4.moveCenter(rect().bottomRight());

    ushort location = 0;
    if(rect1.contains(point))
        location = 1;
    else if(rect2.contains(point))
        location = 2;
    else if(rect3.contains(point))
        location = 3;
    else if(rect4.contains(point))
        location = 4;
    else
        location = 0;
    return location;
}
