#include "hiconitemgroup.h"
#include "hgroupobj.h"
//#include <QObject>
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

HIconItemGroup::HIconItemGroup(HBaseObj* obj, HIconGraphicsItem *parent)
:HIconGraphicsItem(parent),pGroupObj((HGroupObj*)obj)
{

}

HIconItemGroup::~HIconItemGroup()
{

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
    QTransform transform;
    pGroupObj->getTransform(transform,0);
    painter->setTransform(transform,true);
    pGroupObj->paint(painter);
}

int HIconItemGroup::type() const
{
    return enumGroup;
}

void HIconItemGroup::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pointStart = event->scenePos();
    bool bok;
    QTransform trans;
    pGroupObj->getTransform(trans,0);
    QPointF pt = trans.inverted(&bok).map(pointStart);
    pointLocation = pointInRect(pt);
    HIconGraphicsItem::mousePressEvent(event);
}

void HIconItemGroup::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pt = event->scenePos() - pointStart;
    bool bok;
    QTransform trans;
    pGroupObj->getTransform(trans,0);
    QPointF pt1 = trans.inverted(&bok).map(event->scenePos()) - trans.inverted(&bok).map(pointStart);
    qreal deltaX = pt1.x();
    qreal deltaY = pt1.y();
    pointStart = event->scenePos();
    bool bShift = false;
    if(event->modifiers() == Qt::ShiftModifier)
        bShift = true;
    if(pointLocation > 0)
    {
        QRectF rectNew;
        if(pointLocation == 1)
        {
            deltaX = -deltaX;
            deltaY = -deltaY;
        }
        else if(pointLocation == 2)
        {
            deltaY = -deltaY;
        }
        else if(pointLocation == 3)
        {
            deltaX = -deltaX;
        }
        else if(pointLocation == 4)
        {
            //pGroupObj->resetRectPoint(-deltaX,-deltaY);//右下角 扩大，左上角，缩小
        }

        QRectF oldRect = rect();
        rectNew = oldRect.adjusted(-deltaX,-deltaY,deltaX,deltaY);
        double w1 = rectNew.width()/oldRect.width();
        double h1 = rectNew.height()/oldRect.height();
        pGroupObj->resetRectPoint(rectNew.topLeft(),oldRect.topLeft());
        pGroupObj->resize(w1,h1);
        setRect(rectNew.normalized());
    }
    else
    {
        //pGroupObj->moveBy(pt.x(),pt.y());
        //QRectF recttemp = rect().translated(pt.x(),pt.y());
        QRectF recttemp = rect().translated(pt.x(),pt.y());
        setRect(recttemp.normalized());
        //HIconGraphicsItem::mouseMoveEvent(event);
    }
}

void HIconItemGroup::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    HIconGraphicsItem::mouseReleaseEvent(event);
}

void HIconItemGroup::keyPressEvent(QKeyEvent* event)
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

void HIconItemGroup::setRect(const QRectF& rect1)
{
    if(rect1 == rectF) return;
    prepareGeometryChange();
    refreshBaseObj(rect1);
    update();
}

QRectF HIconItemGroup::rect()const
{
    if(pGroupObj)
        return pGroupObj->getObjRect();
    return QRectF();
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

void HIconItemGroup::refreshBaseObj(const QRectF& rect)
{
    pGroupObj->setObjRect(rect);
    QPointF p = rect.center();
    pGroupObj->setOX(p.x());
    pGroupObj->setOY(p.y());
    pGroupObj->setModify(true);
}

//ok
void HIconItemGroup::moveItemBy(qreal dx, qreal dy)
{
    QRectF newRectF;
    newRectF = rect().translated(dx,dy);
    setRect(newRectF);
}

//ok
void HIconItemGroup::resizeItem(const QPolygonF& polygonF)
{
    if(polygonF.size() != 4)
        return;
    //topleft bottomright 
    QRectF rectNew(polygonF.at(0),polygonF.at(3));
    QRectF oldRect = rect();
    if(rectNew.width() < 1 || rectNew.height() < 1)
        return;
    double w1 = rectNew.width()/oldRect.width();
    double h1 = rectNew.height()/oldRect.height();
    pGroupObj->resetRectPoint(rectNew.topLeft(),oldRect.topLeft());
    pGroupObj->resize(w1,h1);
    setRect(rectNew.normalized());

}

//ok
void HIconItemGroup::setItemCursor(int location)
{
    if(location == 1 || location == 4)
        setCursor(QCursor(Qt::SizeFDiagCursor));
    else if(location == 2 || location == 3)
        setCursor(QCursor(Qt::SizeBDiagCursor));
    else
        setCursor(QCursor(Qt::ArrowCursor));
}

//ok
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
