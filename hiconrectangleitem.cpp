#include "hiconrectangleitem.h"
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
#include <QDebug>
HIconRectangleItem::HIconRectangleItem(HIconGraphicsItem *parent)
    :HIconGraphicsItem(parent)
{

}

//ok
HIconRectangleItem::HIconRectangleItem(HBaseObj* obj, HIconGraphicsItem *parent)
    :HIconGraphicsItem(parent),pRectObj((HRectangle*)obj)
{
    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);
}

HIconRectangleItem::~HIconRectangleItem()
{
    /*
    if(pRectObj)
    {
        delete pRectObj;
        pRectObj = NULL;
    }*/
}

QRectF HIconRectangleItem::boundingRect() const
{ 
    return pRectObj->boundingRect();
}

bool HIconRectangleItem::contains(const QPointF &point) const
{
    return pRectObj->contains(point);
}

void HIconRectangleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //transform在此考虑到后面group,iconobj的里面设置transform会重复
    QTransform transform;
    pRectObj->getTransform(transform,0);
    painter->setTransform(transform,true);
    pRectObj->paint(painter);
}

QPainterPath HIconRectangleItem::shape() const
{
    return pRectObj->shape();
}

int HIconRectangleItem::type() const
{
    return enumRectangle;
}

//ok
void HIconRectangleItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pointStart = event->scenePos();
    bool bok;
    QTransform trans;
    pRectObj->getTransform(trans,0);
    QPointF pt = trans.inverted(&bok).map(pointStart);
    pointLocation = pointInRect(pt);
    HIconGraphicsItem::mousePressEvent(event);
}

//ok
void HIconRectangleItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pt = event->scenePos() - pointStart;
    bool bok;
    QTransform trans;
    pRectObj->getTransform(trans,0);
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
        //pRectObj->moveBy(pt.x(),pt.y());
        QRectF recttemp = rect().translated(pt.x(),pt.y());
        setRect(recttemp.normalized());
        if (flags() & ItemIsSelectable)
            setSelected(true);
        //HIconGraphicsItem::mouseMoveEvent(event);
    }
}

//ok
void HIconRectangleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    qDebug()<<" Release before selected:"<<isSelected();
    HIconGraphicsItem::mouseReleaseEvent(event);
    qDebug()<<"Release after selected:"<<isSelected();
}

//ok
void HIconRectangleItem::keyPressEvent(QKeyEvent *event)
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
void HIconRectangleItem::setRect(const QRectF& rect1)
{
    if(rect1 == rect()) return;
    prepareGeometryChange();
    refreshBaseObj(rect1);
    update();
}

QRectF HIconRectangleItem::rect() const
{
    if(pRectObj)
        return pRectObj->getObjRect();
    return QRectF();
}

void HIconRectangleItem::setItemObj(HBaseObj *pObj)
{
    pRectObj = (HRectangle*)pObj;
    pRectObj->setIconGraphicsItem(this);
}

HBaseObj* HIconRectangleItem::getItemObj()
{
    if(pRectObj)
        return pRectObj;
    return NULL;
}

//ok
void HIconRectangleItem::moveItemBy(qreal dx, qreal dy)
{
    QRectF newRectF;
    newRectF = rect().translated(dx,dy);
    setRect(newRectF);
}

//ok
void HIconRectangleItem::resizeItem(const QPolygonF& polygonF)
{
    if(polygonF.size() != 4)
        return;
    //topleft bottomright
    QRectF newRectF(polygonF.at(0),polygonF.at(3));
    setRect(newRectF);
}

void HIconRectangleItem::refreshBaseObj(const QRectF& rect)
{
    pRectObj->setObjRect(rect);
    QPointF p = rect.center();
    pRectObj->setOX(p.x());
    pRectObj->setOY(p.y());
    pRectObj->setModify(true);
}

//ok
void HIconRectangleItem::setItemCursor(int location)
{
    if(location == 1 || location == 4)
        setCursor(QCursor(Qt::SizeFDiagCursor));
    else if(location == 2 || location == 3)
        setCursor(QCursor(Qt::SizeBDiagCursor));
    else
        setCursor(QCursor(Qt::ArrowCursor));
}

//ok
ushort HIconRectangleItem::pointInRect(QPointF& point)
{
    qreal halfpw = 14.00;
    QRectF rect1,rect2,rect3,rect4;
    rect1.setSize(QSizeF(halfpw,halfpw));
    rect1.moveCenter((rect().topLeft()));
    rect2.setSize(QSizeF(halfpw,halfpw));
    rect2.moveCenter((rect().topRight()));
    rect3.setSize(QSizeF(halfpw,halfpw));
    rect3.moveCenter((rect().bottomLeft()));
    rect4.setSize(QSizeF(halfpw,halfpw));
    rect4.moveCenter((rect().bottomRight()));

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

QVariant HIconRectangleItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange) {
             // value is the new position.
             QPointF newPos = value.toPointF();
             QPointF pt = newPos - pointStart;
             QRectF recttemp = rect().translated(pt.x(),pt.y());
             refreshBaseObj(recttemp.normalized());
         }
         return QGraphicsItem::itemChange(change, value);
}
