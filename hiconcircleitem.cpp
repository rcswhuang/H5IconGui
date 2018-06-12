#include "hiconcircleitem.h"
#include "hcircle.h"
//#include <QObject>
#include <QRectF>
#include <QPainterPath>
#include <QPointF>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
HIconCircleItem::HIconCircleItem(HIconRectItem *parent)
    :HIconRectItem(parent)
{

}

HIconCircleItem::HIconCircleItem(const QRectF &rectF, HIconRectItem *parent)
    :HIconRectItem(rectF,parent)
{
    pCircleObj = NULL;
}

HIconCircleItem::~HIconCircleItem()
{
    /*
    if(pCircleObj)
    {
        delete pCircleObj;
        pCircleObj = NULL;
    }*/
}


QRectF HIconCircleItem::boundingRect() const
{
    return pCircleObj->boundingRect();
}

bool HIconCircleItem::contains(const QPointF &point) const
{
    return pCircleObj->contains(point);
}

void HIconCircleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QTransform transform;
    pCircleObj->getTransform(transform,0);
    painter->setTransform(transform,true);
    pCircleObj->paint(painter);
}

QPainterPath HIconCircleItem::shape() const
{
    return pCircleObj->shape();
}

int HIconCircleItem::type() const
{
    return enumCircle;
}


void HIconCircleItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pointStart = event->scenePos();
    bool bok;
    QTransform trans;
    pCircleObj->getTransform(trans,0);
    QPointF pt = trans.inverted(&bok).map(pointStart);
    pointLocation = pointInRect(pt);
    HIconGraphicsItem::mousePressEvent(event);
}

void HIconCircleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{ 
    QPointF pt = event->scenePos() - pointStart;
    bool bok;
    QTransform trans;
    pCircleObj->getTransform(trans,0);
    QPointF pt1 = trans.inverted(&bok).map(event->scenePos());
    pointStart = event->scenePos();
    bool bShift = false;
    if(event->modifiers() == Qt::ShiftModifier)
        bShift = true;
    if(pointLocation == 1)
    {
        QRectF rectNew;
        rectNew.setTopLeft(pt1);
        rectNew.setBottomRight(rect().bottomRight());
        setRect(rectNew.normalized());
    }
    else if(pointLocation == 2)
    {
        QRectF rectNew;
        rectNew.setTopRight(pt1);
        rectNew.setBottomLeft(rect().bottomLeft());
        setRect(rectNew.normalized());
    }
    else if(pointLocation == 3)
    {
        QRectF rectNew;
        rectNew.setBottomLeft(pt1);
        rectNew.setTopRight(rect().topRight());
        setRect(rectNew.normalized());
    }
    else if(pointLocation == 4)
    {
        QRectF rectNew;
        rectNew.setBottomRight(pt1);
        rectNew.setTopLeft(rect().topLeft());
        setRect(rectNew.normalized());
    }
    else
    {
        pCircleObj->moveBy(pt.x(),pt.y());
        QRectF recttemp = pCircleObj->getObjRect();
        setRect(recttemp.normalized());
    }
}

/*
void HIconCircleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    HIconGraphicsItem::mouseReleaseEvent(event);
}

void HIconCircleItem::keyPressEvent(QKeyEvent *event)
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
*/
void HIconCircleItem::setRect(const QRectF& rect)
{
    if(rect == rectF) return;
    prepareGeometryChange();
    rectF = rect;
    refreshBaseObj(rect);
    update();
}

QRectF HIconCircleItem::rect()const
{
    return rectF;
}

void HIconCircleItem::setItemObj(HBaseObj *pObj)
{
    pCircleObj = (HCircle*)pObj;
    pCircleObj->setIconGraphicsItem(this);
}

HBaseObj* HIconCircleItem::getItemObj()
{
    if(pCircleObj)
        return pCircleObj;
    return NULL;
}

/*
void HIconCircleItem::moveItemBy(qreal dx, qreal dy)
{
    QRectF newRectF;
    newRectF = rect().translated(dx,dy);
    setRect(newRectF);
}

void HIconCircleItem::resizeItem(const QPolygonF& polygonF)
{
    if(polygonF.size() != 2)
        return;
    QRectF newRectF(polygonF.at(0),polygonF.at(1));
    setRect(newRectF);
}
*/

void HIconCircleItem::refreshBaseObj(const QRectF& rect)
{
    pCircleObj->setObjRect(rect);
    QPointF p = rect.center();
    pCircleObj->setOX(p.x());
    pCircleObj->setOY(p.y());
    pCircleObj->setModify(true);
}

/*
ushort HIconCircleItem::pointInRect(QPointF& point)
{
    qreal halfpw = 14.00;
    QRectF rect1,rect2,rect3,rect4;
    rect1.setSize(QSizeF(halfpw,halfpw));
    rect1.moveCenter(mapToScene(rect().topLeft()));
    rect2.setSize(QSizeF(halfpw,halfpw));
    rect2.moveCenter(mapToScene(rect().topRight()));
    rect3.setSize(QSizeF(halfpw,halfpw));
    rect3.moveCenter(mapToScene(rect().bottomLeft()));
    rect4.setSize(QSizeF(halfpw,halfpw));
    rect4.moveCenter(mapToScene(rect().bottomRight()));

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

void HIconCircleItem::setItemCursor(int location)
{
    if(location == 1 || location == 4)
        setCursor(QCursor(Qt::SizeFDiagCursor));
    else if(location == 2 || location == 3)
        setCursor(QCursor(Qt::SizeBDiagCursor));
    else
        setCursor(QCursor(Qt::ArrowCursor));
}
*/

