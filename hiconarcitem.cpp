#include "hiconarcitem.h"
#include "harc.h"
//#include <QObject>
#include <QRectF>
#include <QPainterPath>
#include <QPointF>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
HIconArcItem::HIconArcItem(HIconRectangleItem *parent)
    :HIconRectangleItem(parent)
{

}

HIconArcItem::HIconArcItem(HBaseObj* obj, HIconRectangleItem *parent)
    :pArcObj((HArc*)obj),HIconRectangleItem(parent)
{

}

HIconArcItem::~HIconArcItem()
{
    /*
    if(pArcObj)
    {
        delete pArcObj;
        pArcObj = NULL;
    }*/
}


QRectF HIconArcItem::boundingRect() const
{
    return pArcObj->boundingRect();
}

bool HIconArcItem::contains(const QPointF &point) const
{
    return pArcObj->contains(point);
}

void HIconArcItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QTransform transform;
    pArcObj->getTransform(transform,0);
    painter->setTransform(transform,true);
    pArcObj->paint(painter);
}

QPainterPath HIconArcItem::shape() const
{
    return pArcObj->shape();

}

int HIconArcItem::type() const
{
    return enumArc;
}

void HIconArcItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pointStart = event->scenePos();
    bool bok;
    QTransform trans;
    pArcObj->getTransform(trans,0);
    QPointF pt = trans.inverted(&bok).map(pointStart);
    pointLocation = pointInRect(pt);
    HIconGraphicsItem::mousePressEvent(event);
}

void HIconArcItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{ 
    QPointF pt = event->scenePos() - pointStart;
    bool bok;
    QTransform trans;
    pArcObj->getTransform(trans,0);
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
        pArcObj->moveBy(pt.x(),pt.y());
        QRectF recttemp = pArcObj->getObjRect();
        setRect(recttemp.normalized());
    }
}

/*
void HIconArcItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    HIconGraphicsItem::mouseReleaseEvent(event);
}
*/
/*
void HIconArcItem::keyPressEvent(QKeyEvent *event)
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
}*/

void HIconArcItem::setRect(const QRectF& rect1)
{
    if(rect1 == rect()) return;
    prepareGeometryChange();
    refreshBaseObj(rect1);
    update();
}

QRectF HIconArcItem::rect()const
{
    if(pArcObj)
        return pArcObj->getObjRect();
    return QRectF();
}

void HIconArcItem::setItemObj(HBaseObj *pObj)
{
    pArcObj = (HArc*)pObj;
    pArcObj->setIconGraphicsItem(this);
}

HBaseObj* HIconArcItem::getItemObj()
{
    if(pArcObj)
        return pArcObj;
    return NULL;
}

/*
void HIconArcItem::moveItemBy(qreal dx, qreal dy)
{
    //主要是记录HBaseObj的位置信息，图元移动的信息由mousemoveevent来解决
    QRectF newRectF;
    newRectF = rect().translated(dx,dy);
    setRect(newRectF);
}
*/
void HIconArcItem::refreshBaseObj(const QRectF& rect)
{
    pArcObj->setObjRect(rect);
    QPointF p = rect.center();
    pArcObj->setOX(p.x());
    pArcObj->setOY(p.y());
    pArcObj->setModify(true);
}

/*
void HIconArcItem::resizeItem(const QPolygonF& polygonF)
{
    if(polygonF.size() != 2)
        return;
    //topleft bottomright
    QRectF newRectF(polygonF.at(0),polygonF.at(1));
    setRect(newRectF);
}

void HIconArcItem::setItemCursor(int location)
{
    if(location == 1 || location == 4)
        setCursor(QCursor(Qt::SizeFDiagCursor));
    else if(location == 2 || location == 3)
        setCursor(QCursor(Qt::SizeBDiagCursor));
    else
        setCursor(QCursor(Qt::ArrowCursor));
}

ushort HIconArcItem::pointInRect(QPointF& point)
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
}*/
