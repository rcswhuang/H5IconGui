#include "H5IconGui/hicontextitem.h"
#include "H5IconGui/hiconrectobj.h"
#include <QObject>
#include <QRectF>
#include <QPainterPath>
#include <QPointF>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QFont>
HIconTextItem::HIconTextItem(HIconRectItem *parent)
    :HIconRectItem(parent)
{

}

HIconTextItem::HIconTextItem(const QRectF &rectF, HIconRectItem *parent)
    :HIconRectItem(rectF,parent)
{
    pTextObj =  NULL;
}

QRectF HIconTextItem::boundingRect() const
{
    return pTextObj->boundingRect();
}

bool HIconTextItem::contains(const QPointF &point) const
{
    return pTextObj->contains(point);
}

void HIconTextItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    pTextObj->paint(painter);
}

QPainterPath HIconTextItem::shape() const
{
    return pTextObj->shape();
}

int HIconTextItem::type() const
{
    return enumText;
}

/*
void HIconTextItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pointStart = event->scenePos();
    pointLocation = pointInRect(pointStart);
    HIconGraphicsItem::mousePressEvent(event);
}
*/

void HIconTextItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qreal fRotateAngle = pTextObj->getRotateAngle();
    QTransform transform;
    transform.rotate(-fRotateAngle);
    QPointF pt = transform.map(event->scenePos()) - transform.map(pointStart);
    transform.rotate(fRotateAngle);
    pointStart = event->scenePos();
    bool bShift = false;
    if(event->modifiers() == Qt::ShiftModifier)
        bShift = true;
    if(pointLocation == 1)
    {
        QRectF rectNew;
        rectNew.setTopLeft(QPointF(rect().left() + pt.x(),rect().top() + pt.y()));
        rectNew.setBottomRight(rect().bottomRight());
        setRect(rectNew.normalized());
    }
    else if(pointLocation == 2)
    {
        QRectF rectNew;
        rectNew.setTopRight(QPointF(rect().right() + pt.x(),rect().top() + pt.y()));
        rectNew.setBottomLeft(rect().bottomLeft());
        setRect(rectNew.normalized());
    }
    else if(pointLocation == 3)
    {
        QRectF rectNew;
        rectNew.setBottomLeft(QPointF(rect().left() + pt.x(),rect().bottom() + pt.y()));
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
        pTextObj->moveBy(pt.x(),pt.y());
        QRectF recttemp = pTextObj->getObjRect();//(pTextObj->getTopLeftPoint(),QSize(pTextObj->getRectWidth(),pTextObj->getRectHeight()));
        setRect(recttemp.normalized());
    }
}

/*
void HIconTextItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    HIconGraphicsItem::mouseReleaseEvent(event);
}

void HIconTextItem::keyPressEvent(QKeyEvent *event)
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

void HIconTextItem::setRect(const QRectF& rect)
{
    if(rect == rectF) return;
    prepareGeometryChange();
    rectF = rect;
    refreshBaseObj(rect);
    update();
}

QRectF HIconTextItem::rect()const
{
    return rectF;
}

void HIconTextItem::setItemObj(HBaseObj *pObj)
{
    pTextObj = (HTextObj*)pObj;
    pTextObj->setIconGraphicsItem(this);
}

HBaseObj* HIconTextItem::getItemObj()
{
    if(pTextObj)
        return pTextObj;
    return NULL;
}

/*
void HIconTextItem::moveItemBy(qreal dx, qreal dy)
{
    QRectF newRectF;
    newRectF = rect().translated(dx,dy);
    setRect(newRectF);
}

void HIconTextItem::resizeItem(const QPolygonF& polygonF)
{
    if(polygonF.size() != 2)
        return;
    //topleft bottomright
    QRectF newRectF(polygonF.at(0),polygonF.at(1));
    setRect(newRectF);
}
*/

void HIconTextItem::refreshBaseObj(const QRectF& rect)
{
    pTextObj->setObjRect(rect);
    QPointF p = rect.center();
    pTextObj->setOX(p.x());
    pTextObj->setOY(p.y());
    pTextObj->setModify(true);
}

/*
void HIconTextItem::setItemCursor(int location)
{
    if(location == 1 || location == 4)
        setCursor(QCursor(Qt::SizeFDiagCursor));
    else if(location == 2 || location == 3)
        setCursor(QCursor(Qt::SizeBDiagCursor));
    else
        setCursor(QCursor(Qt::ArrowCursor));
}

ushort HIconTextItem::pointInRect(QPointF& point)
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
