#include "H5IconGui/hiconitemgroup.h"
#include "H5IconGui/hgroupobj.h"
#include <qmath.h>
#include <QObject>
#include <QRectF>
#include <QPainterPath>
#include <QPointF>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
HIconItemGroup::HIconItemGroup(HIconGraphicsItem *parent)
{

}

HIconItemGroup::HIconItemGroup(const QRectF &rectF, HIconGraphicsItem *parent)
:HIconGraphicsItem(parent),rectF(rectF)
{
    setFlag(QGraphicsItem::ItemIsMovable,true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);
    setFlag(QGraphicsItem::ItemIsFocusable,true);
    pGroupObj = NULL;
}

QRectF HIconItemGroup::boundingRect() const
{
    return pGroupObj->boundingRect();
}

bool HIconItemGroup::contains(const QPointF &point) const
{
    return pGroupObj->contains(point);
}

QPainterPath HIconItemGroup::shape() const
{
    return pGroupObj->shape();
}

void HIconItemGroup::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    pGroupObj->paint(painter);
}

int HIconItemGroup::type() const
{
    return enumGroup;
}

void HIconItemGroup::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pointStart = event->scenePos();
    pointLocation = pointInRect(pointStart);
    HIconGraphicsItem::mousePressEvent(event);
}

void HIconItemGroup::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pt = event->scenePos() - pointStart;
    qreal deltaX =pt.x();
    qreal deltaY = pt.y();
    pointStart = event->scenePos();
    bool bShift = false;
    if(event->modifiers() == Qt::ShiftModifier)
        bShift = true;
    if(pointLocation > 0)
    {
        QRectF rectNew;
        if(pointLocation == 1)
        {
            pGroupObj->resetRectPoint(deltaX,deltaY);
        }
        else if(pointLocation == 2)
        {
            pGroupObj->resetRectPoint(-deltaX,deltaY);
        }
        else if(pointLocation == 3)
        {
            pGroupObj->resetRectPoint(deltaX,-deltaY);
        }
        else if(pointLocation == 4)
        {
            pGroupObj->resetRectPoint(-deltaX,-deltaY);//右下角 扩大，左上角，缩小
        }
        rectNew = rectF.adjusted(-deltaX,-deltaY,deltaX,deltaY);

        double w1 = rectNew.width()/rectF.width();
        double h1 = rectNew.height()/rectF.height();
        pGroupObj->resize(w1,h1);

        rectNew = pGroupObj->getObjRect();
        setRect(rectNew.normalized());
    }
    else
    {
        pGroupObj->moveBy(pt.x(),pt.y());
        QRectF recttemp(pGroupObj->getTopLeft().x(),pGroupObj->getTopLeft().y(),pGroupObj->getGroupWidth(),pGroupObj->getGroupHeight());
        setRect(recttemp.normalized());
        //HIconGraphicsItem::mouseMoveEvent(event);
    }
}

void HIconItemGroup::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    HIconGraphicsItem::mouseReleaseEvent(event);
}

void HIconItemGroup::keyPressEvent(QKeyEvent *event)
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

void HIconItemGroup::setRect(const QRectF& rect)
{
    if(rect == rectF) return;
    prepareGeometryChange();

    rectF = rect;
    refreshBaseObj(rect);
    update();
}

QRectF HIconItemGroup::rect()const
{
    return rectF;
}

void HIconItemGroup::setItemObj(HBaseObj* pObj)
{
    pGroupObj = (HGroupObj*)pObj;
    pGroupObj->setIconGraphicsItem(this);
}

HBaseObj* HIconItemGroup::getItemObj()
{
    return pGroupObj;
}

void HIconItemGroup::moveItemBy(qreal dx, qreal dy)
{
    QRectF newRectF;
    newRectF = rect().translated(dx,dy);
    setRect(newRectF);
}

void HIconItemGroup::resizeItem(const QPolygonF& polygonF)
{
    if(polygonF.size() != 4)
        return;
    //topleft bottomright
    QRectF newRectF(polygonF.at(0),polygonF.at(3));
    setRect(newRectF);
}

void HIconItemGroup::refreshBaseObj(const QRectF& rect)
{
    //pGroupObj->setTopLeft(QPointF(rect.x(),rect.y()));
    //pGroupObj->setGroupWidth(rect.width());
    //pGroupObj->setGroupHeight(rect.height());
    pGroupObj->setObjRect(rect);
    QPointF p = rect.center();
    pGroupObj->setOX(p.x());
    pGroupObj->setOY(p.y());
    pGroupObj->setModify(true);
}

void HIconItemGroup::setItemCursor(int location)
{
    if(location == 1 || location == 4)
        setCursor(QCursor(Qt::SizeFDiagCursor));
    else if(location == 2 || location == 3)
        setCursor(QCursor(Qt::SizeBDiagCursor));
    else
        setCursor(QCursor(Qt::ArrowCursor));
}

ushort HIconItemGroup::pointInRect(QPointF& point)
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

void HIconItemGroup::resetRectPoint(qreal dx, qreal dy)
{
    //if(pGroupObj)
      //  pGroupObj->setTopLeftPoint(point);
}




