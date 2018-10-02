#include "hiconellipseitem.h"
#include "hellipse.h"
//#include <QObject>
#include <QRectF>
#include <QPainterPath>
#include <QPointF>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
HIconEllipseItem::HIconEllipseItem(HIconRectangleItem *parent)
    :HIconRectangleItem(parent)
{

}

HIconEllipseItem::HIconEllipseItem(HBaseObj* obj, HIconRectangleItem *parent)
    :pEllipseObj((HEllipse*)obj),HIconRectangleItem(parent)
{
}

HIconEllipseItem::~HIconEllipseItem()
{
}

QRectF HIconEllipseItem::boundingRect() const
{
    return pEllipseObj->boundingRect();
}

bool HIconEllipseItem::contains(const QPointF &point) const
{
    return pEllipseObj->contains(point);
}

void HIconEllipseItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QTransform transform;
    pEllipseObj->getTransform(transform,0);
    painter->setTransform(transform,true);
    pEllipseObj->paint(painter);
}

QPainterPath HIconEllipseItem::shape() const
{
    return pEllipseObj->shape();
}

int HIconEllipseItem::type() const
{
    return enumEllipse;
}


void HIconEllipseItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pointStart = event->scenePos();
    bool bok;
    QTransform trans;
    pEllipseObj->getTransform(trans,0);
    QPointF pt = trans.inverted(&bok).map(pointStart);
    pointLocation = pointInRect(pt);
    HIconGraphicsItem::mousePressEvent(event);
}

void HIconEllipseItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pt = event->scenePos() - pointStart;
    bool bok;
    QTransform trans;
    pEllipseObj->getTransform(trans,0);
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
        //pEllipseObj->moveBy(pt.x(),pt.y());
        //QRectF recttemp = pEllipseObj->getObjRect();
        QRectF recttemp = rect().translated(pt.x(),pt.y());
        setRect(recttemp.normalized());
        if (flags() & ItemIsSelectable)
            setSelected(true);
    }
}

/*
void HIconEllipseItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    HIconGraphicsItem::mouseReleaseEvent(event);
}

void HIconEllipseItem::keyPressEvent(QKeyEvent *event)
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

void HIconEllipseItem::setRect(const QRectF& rect1)
{
    if(rect1 == rect()) return;
    prepareGeometryChange();
    refreshBaseObj(rect1);
    update();
}

QRectF HIconEllipseItem::rect()const
{
    if(pEllipseObj)
        return pEllipseObj->getObjRect();
    return QRectF();
}

void HIconEllipseItem::setItemObj(HBaseObj *pObj)
{
    pEllipseObj = (HEllipse*)pObj;
    pEllipseObj->setIconGraphicsItem(this);

}

HBaseObj* HIconEllipseItem::getItemObj()
{
    if(pEllipseObj)
        return pEllipseObj;
    return NULL;
}

/*
void HIconEllipseItem::moveItemBy(qreal dx, qreal dy)
{
    QRectF newRectF;
    newRectF = rect().translated(dx,dy);
    setRect(newRectF);
}

void HIconEllipseItem::resizeItem(const QPolygonF& polygonF)
{
    if(polygonF.size() != 2)
        return;
    //topleft bottomright
    QRectF newRectF(polygonF.at(0),polygonF.at(1));
    setRect(newRectF);
}
*/
void HIconEllipseItem::refreshBaseObj(const QRectF& rect)
{
    pEllipseObj->setObjRect(rect);
    QPointF p = rect.center();
    pEllipseObj->setOX(p.x());
    pEllipseObj->setOY(p.y());
    pEllipseObj->setModify(true);
}

/*
ushort HIconEllipseItem::pointInRect(QPointF& point)
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

void HIconEllipseItem::setItemCursor(int location)
{
    if(location == 1 || location == 4)
        setCursor(QCursor(Qt::SizeFDiagCursor));
    else if(location == 2 || location == 3)
        setCursor(QCursor(Qt::SizeBDiagCursor));
    else
        setCursor(QCursor(Qt::ArrowCursor));
}
*/
