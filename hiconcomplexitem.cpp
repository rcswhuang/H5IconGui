#include "H5IconGui/hiconcomplexitem.h"
#include "H5IconGui/hiconobj.h"
#include "H5IconGui/hiconshowpattern.h"
#include <qmath.h>
#include <QObject>
#include <QRectF>
#include <QPainterPath>
#include <QPointF>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QStyleOptionGraphicsItem>
#include <QPainter>

HIconComplexItem::HIconComplexItem(HIconGraphicsItem *parent)
    :HIconRectItem(parent)
{

}

HIconComplexItem::HIconComplexItem(const QRectF &rectF, HIconGraphicsItem *parent)
    :HIconRectItem(rectF,parent)
{
    setFlag(QGraphicsItem::ItemIsMovable,true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);
    setFlag(QGraphicsItem::ItemIsFocusable,true);
    setAcceptDrops(true);
    pIconComplexObj = NULL;
}

QRectF HIconComplexItem::boundingRect() const
{ 
    return pIconComplexObj->boundingRect();
}

bool HIconComplexItem::contains(const QPointF &point) const
{
    return pIconComplexObj->contains(point);
}

void HIconComplexItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    //尚未确定
    pIconComplexObj->paint(painter);

}

QPainterPath HIconComplexItem::shape() const
{
    return pIconComplexObj->shape();
}

int HIconComplexItem::type() const
{
    return enumComplex;
}
/*
void HIconComplexItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pointStart = event->scenePos();
    pointLocation = pointInRect(pointStart);
    HIconGraphicsItem::mousePressEvent(event);
}*/

void HIconComplexItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    //每次移动或者改变大小 都要刷新到里面的所有部分
    /*qreal fRotateAngle = pIconComplexObj->getRotateAngle();
    QTransform transform;
    transform.rotate(-fRotateAngle);
    QPointF pt = transform.map(event->scenePos()) - transform.map(pointStart);
    transform.rotate(fRotateAngle);*/

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
        rectNew = rectNew.normalized();
        double w1 = rectNew.width()/rectF.width();
        if(w1 < 0.000001)
            w1 = 1;
        double h1 = rectNew.height()/rectF.height();
        pIconComplexObj->resetRectPoint(rectNew.topLeft(),rectF.topLeft());
        pIconComplexObj->resize(w1,h1);
        setRect(rectNew);
    }
    else
    {
        pIconComplexObj->moveBy(pt.x(),pt.y());
        QRectF recttemp(pIconComplexObj->getObjRect());
        setRect(recttemp.normalized());
        //HIconGraphicsItem::mouseMoveEvent(event);
    }
}

/*
void HIconComplexItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    HIconGraphicsItem::mouseReleaseEvent(event);
}

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
    QRectF newRect = rect().adjusted(ndx,ndy,ndx,ndy);
    setRect(newRect);
}
*/
void HIconComplexItem::setRect(const QRectF& rect)
{
    if(rect == rectF) return;
    prepareGeometryChange();
    rectF = rect;
    refreshBaseObj(rect);
    update();
}

QRectF HIconComplexItem::rect()const
{
    return rectF;
}

void HIconComplexItem::setItemObj(HBaseObj *pObj)
{
    pIconComplexObj = (HIconComplexObj*)pObj;
    pIconComplexObj->setIconGraphicsItem(this);
}

HBaseObj* HIconComplexItem::getItemObj()
{
    if(pIconComplexObj)
        return pIconComplexObj;
    return NULL;
}


void HIconComplexItem::moveItemBy(qreal dx, qreal dy)
{
    QRectF newRectF;
    newRectF = rect().translated(dx,dy);
    pIconComplexObj->moveBy(dx,dy);
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
    pIconComplexObj->setObjRect(rect);
    QPointF p = rect.center();
    pIconComplexObj->setOX(p.x());
    pIconComplexObj->setOY(p.y());
    //pIconComplexObj->resize(rect().width(),rect().height());
    pIconComplexObj->setModify(true);

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
