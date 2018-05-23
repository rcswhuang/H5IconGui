#include "hiconselectionitem.h"
//#include "H5IconGui/hiconobj.h"
//#include <QObject>
#include <QRectF>
#include <QPainterPath>
#include <QPointF>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QPen>
#include "hbaseobj.h"
#include "hiconobj.h"
HIconSelectionItem::HIconSelectionItem(HIconGraphicsItem *parent)
    :HIconGraphicsItem(parent)
{

}

HIconSelectionItem::HIconSelectionItem(const QRectF &rectF, HIconGraphicsItem *parent)
    :HIconGraphicsItem(parent),rectF(rectF)
{
}

HIconSelectionItem::~HIconSelectionItem()
{
    //清空，未删除
    clear();
}

void HIconSelectionItem::clear()
{
    pObjList.clear();
}

QRectF HIconSelectionItem::boundingRect() const
{
    return shape().controlPointRect();
}

bool HIconSelectionItem::contains(const QPointF &point) const
{
    return shape().contains(point);
}

void HIconSelectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    QPen pen = QPen(QColor(Qt::green));
    pen.setWidth(1);
    pen.setStyle(Qt::DashLine);
    painter->setPen(pen);
    painter->drawRect(rect());
    painter->restore();

    if(isSelected())
    {
        painter->save();
        QPen pen1 = QPen(Qt::red);
        pen1.setWidth(1);
        QBrush brush;
        brush.setColor(Qt::green);
        brush.setStyle(Qt::SolidPattern);
        painter->setPen(pen1);
        painter->setBrush(brush);

        QRectF rect = this->rect();

        qreal halfpw = 8.00;
        QRectF rect1,rect2,rect3,rect4;
        rect1.setSize(QSizeF(halfpw,halfpw));
        rect1.moveCenter(rect.topLeft());
        rect2.setSize(QSizeF(halfpw,halfpw));
        rect2.moveCenter(rect.topRight());
        rect3.setSize(QSizeF(halfpw,halfpw));
        rect3.moveCenter(rect.bottomLeft());
        rect4.setSize(QSizeF(halfpw,halfpw));
        rect4.moveCenter(rect.bottomRight());


        QRectF rectC1,rectC2,rectC3,rectC4; //上中，下中，左中，右中
        rectC1.setSize(QSizeF(halfpw,halfpw));
        rectC2.setSize(QSizeF(halfpw,halfpw));
        rectC3.setSize(QSizeF(halfpw,halfpw));
        rectC4.setSize(QSizeF(halfpw,halfpw));
        QPointF ptC1 = QPointF((rect.topLeft().x()+rect.topRight().x())/2,rect.top());
        QPointF ptC2 = QPointF((rect.bottomLeft().x()+rect.bottomRight().x())/2,rect.bottom());
        QPointF ptC3 = QPointF(rect.left(),(rect.topLeft().y()+rect.bottomLeft().y())/2);
        QPointF ptC4 = QPointF(rect.right(),(rect.topRight().y()+rect.bottomRight().y())/2);

        rectC1.moveCenter(ptC1);
        rectC2.moveCenter(ptC2);
        rectC3.moveCenter(ptC3);
        rectC4.moveCenter(ptC4);

        painter->drawRect(rect1);
        painter->drawRect(rect2);
        painter->drawRect(rect3);
        painter->drawRect(rect4);

        painter->drawRect(rectC1);
        painter->drawRect(rectC2);
        painter->drawRect(rectC3);
        painter->drawRect(rectC4);

        painter->restore();
    }
}

QPainterPath HIconSelectionItem::shape() const
{
    QPainterPath path;
    QRectF boundingRect = rect().adjusted(-5,-5,5,5);
    path.addRect(boundingRect);
    return path;
}

int HIconSelectionItem::type() const
{
    return enumSelection;
}

void HIconSelectionItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pointStart = event->scenePos();
    pointLocation = pointInRect(pointStart);
    HIconGraphicsItem::mousePressEvent(event);
}

void HIconSelectionItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
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
        double w1 = rectNew.width()/rectF.width();
        double h1 = rectNew.height()/rectF.height();
        resetRectPoint(rectNew.topLeft(),rectF.topLeft());
        resize(w1,h1);
        setRect(rectNew.normalized());
    }
    else
    {
        moveBy(pt.x(),pt.y());
        QPointF topLeft =QPointF(rect().topLeft().x() + pt.x(),rect().topLeft().y() + pt.y());
        QRectF recttemp(topLeft,QSizeF(rect().width(),rect().height()));
        setRect(recttemp.normalized());
    }
}

void HIconSelectionItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    HIconGraphicsItem::mouseReleaseEvent(event);
}

void HIconSelectionItem::keyPressEvent(QKeyEvent* event)
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

void HIconSelectionItem::setRect(const QRectF& rect)
{
    if(rect == rectF) return;
    prepareGeometryChange();
    rectF = rect;
    update();
}

QRectF HIconSelectionItem::rect()const
{
    return rectF;
}

//需要设置4个点的位置
void HIconSelectionItem::resetRectPoint(const QPointF& pt1,const QPointF& pt2)
{
    for(int i = 0; i < pObjList.count();i++)
    {
        HBaseObj* pObj = (HBaseObj*)pObjList[i];
        pObj->resetRectPoint(pt1,pt2);
    }
}

void HIconSelectionItem::resize(double w,double h)
{
    for(int i = 0; i < pObjList.count();i++)
    {
        HBaseObj* pObj = (HBaseObj*)pObjList[i];
        pObj->resize(w,h);
    }
}

//ok
ushort HIconSelectionItem::pointInRect(QPointF& point)
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

void HIconSelectionItem::moveBy(qreal dx,qreal dy)
{
    for(int i = 0; i < pObjList.count();i++)
    {
        HBaseObj* pObj = (HBaseObj*)pObjList[i];
        pObj->moveBy(dx,dy);
    }
}

//ok
void HIconSelectionItem::setItemCursor(int location)
{
    if(location == 1 || location == 4)
        setCursor(QCursor(Qt::SizeFDiagCursor));
    else if(location == 2 || location == 3)
        setCursor(QCursor(Qt::SizeBDiagCursor));
    else
        setCursor(QCursor(Qt::ArrowCursor));
}
