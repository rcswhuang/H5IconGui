#include "hiconcomplexitem.h"
#include "hiconobj.h"
#include "hiconshowpattern.h"
#include <qmath.h>
//#include <QObject>
#include <QRectF>
#include <QPainterPath>
#include <QPointF>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QStyleOptionGraphicsItem>
#include <QPainter>

HIconComplexItem::HIconComplexItem(HIconGraphicsItem *parent)
    :HIconRectangleItem(parent)
{

}

HIconComplexItem::HIconComplexItem(HBaseObj* obj, HIconGraphicsItem *parent)
    :pIconObj((HIconObj*)obj),HIconRectangleItem(parent)
{
    setAcceptDrops(true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);
}

HIconComplexItem::~HIconComplexItem()
{

}

QRectF HIconComplexItem::boundingRect() const
{ 
    return pIconObj->boundingRect();
}

bool HIconComplexItem::contains(const QPointF &point) const
{
    return pIconObj->contains(point);
}

void HIconComplexItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QTransform transform;
    pIconObj->getTransform(transform,0);
    painter->setTransform(transform,true);
    pIconObj->paint(painter);

}

QPainterPath HIconComplexItem::shape() const
{
    return pIconObj->shape();
}

int HIconComplexItem::type() const
{
    return enumComplex;
}

void HIconComplexItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pointStart = event->scenePos();
    bool bok;
    QTransform trans;
    pIconObj->getTransform(trans,0);
    QPointF pt = trans.inverted(&bok).map(pointStart);
    pointLocation = pointInRect(pt);
    HIconGraphicsItem::mousePressEvent(event);
}

void HIconComplexItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pt = event->scenePos() - pointStart;
    bool bok;
    QTransform trans;
    pIconObj->getTransform(trans,0);
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

        rectNew = rectF.adjusted(-deltaX,-deltaY,deltaX,deltaY);
        if(rectNew.width() < 1 || rectNew.height() < 1)
            return;

        double w1 = rectNew.width()/rectF.width();
        double h1 = rectNew.height()/rectF.height();
        pIconObj->resetRectPoint(rectNew.topLeft(),rectF.topLeft());
        pIconObj->resize(w1,h1);
        setRect(rectNew.normalized());

    }
    else
    {
        pIconObj->moveBy(pt.x(),pt.y());
        QRectF recttemp(pIconObj->getObjRect());
        setRect(recttemp.normalized());
        //HIconGraphicsItem::mouseMoveEvent(event);
    }
}

/*
void HIconComplexItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    HIconGraphicsItem::mouseReleaseEvent(event);
}*/


void HIconComplexItem::keyPressEvent(QKeyEvent *event)
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
    pIconObj->moveBy(ndx,ndy);
    QRectF newRect = rect().adjusted(ndx,ndy,ndx,ndy);
    setRect(newRect);
}

void HIconComplexItem::setRect(const QRectF& rect1)
{
    if(rect1 == rect()) return;
    prepareGeometryChange();
    refreshBaseObj(rect1);
    update();
}

QRectF HIconComplexItem::rect()const
{
    if(pIconObj)
        return pIconObj->getObjRect();
    return QRectF();
}

void HIconComplexItem::setItemObj(HBaseObj *pObj)
{
    pIconObj = (HIconObj*)pObj;
    pIconObj->setIconGraphicsItem(this);
}

HBaseObj* HIconComplexItem::getItemObj()
{
    if(pIconObj)
        return pIconObj;
    return NULL;
}


void HIconComplexItem::moveItemBy(qreal dx, qreal dy)
{
    QRectF newRectF;
    newRectF = rect().translated(dx,dy);
    pIconObj->moveBy(dx,dy);
    setRect(newRectF);
}
/*
void HIconComplexItem::resizeItem(const QPolygonF& polygonF)
{
    if(polygonF.size() != 4)
        return;
    //topleft bottomright
    QRectF newRectF(polygonF.at(0),polygonF.at(3));
    setRect(newRectF);
}
*/
void HIconComplexItem::refreshBaseObj(const QRectF& rect)
{
    //注意所有的对象都要刷新一下
    pIconObj->setObjRect(rect);
    QPointF p = rect.center();
    pIconObj->setOX(p.x());
    pIconObj->setOY(p.y());
    //pIconComplexObj->resize(rect().width(),rect().height());
    pIconObj->setModify(true);

}

/*
void HIconComplexItem::setItemCursor(int location)
{
    if(location == 1 || location == 4)
        setCursor(QCursor(Qt::SizeFDiagCursor));
    else if(location == 2 || location == 3)
        setCursor(QCursor(Qt::SizeBDiagCursor));
    else
        setCursor(QCursor(Qt::ArrowCursor));
}

ushort HIconComplexItem::pointInRect(QPointF& point)
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
*/
