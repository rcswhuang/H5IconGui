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
    qreal fRotateAngle = pIconComplexObj->getRotateAngle();
    QTransform transform;
    transform.rotate(-fRotateAngle);
    QPointF pt = transform.map(event->scenePos()) - transform.map(pointStart);
    transform.rotate(fRotateAngle);


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
        pIconComplexObj->moveBy(pt.x(),pt.y());
        HIconGraphicsItem::mouseMoveEvent(event);
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
