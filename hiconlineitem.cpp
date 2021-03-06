#include "hiconlineitem.h"
#include "hline.h"
#include <math.h>
//#include <QObject>
#include <QRectF>
#include <QPainterPath>
#include <QPointF>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QStyleOptionGraphicsItem>
#include <QPainter>
#include <QDebug>
HIconLineItem::HIconLineItem(HIconGraphicsItem *parent):HIconGraphicsItem(parent)
{
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    lineF.setLine(0,0,0,0);
}

HIconLineItem::HIconLineItem(HBaseObj* obj, HIconGraphicsItem *parent):HIconGraphicsItem(parent),pLineObj((HLine*)obj)
{
}

HIconLineItem::~HIconLineItem()
{
    /*
    if(pLineObj)
    {
        delete pLineObj;
        pLineObj = NULL;
    }*/
}

QRectF HIconLineItem::boundingRect() const
{
    return shape().controlPointRect();
}

bool HIconLineItem::contains(const QPointF &point) const
{
    return shape().boundingRect().contains(point);
}

void HIconLineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QTransform transform;
    pLineObj->getTransform(transform,0);
    painter->setTransform(transform,true);
    pLineObj->paint(painter); 
}

QPainterPath HIconLineItem::shape() const
{
   return pLineObj->shape();
}

int HIconLineItem::type() const
{
    return enumLine;
}

void HIconLineItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    curPointF = event->scenePos();
    bool bok;
    QTransform trans;
    pLineObj->getTransform(trans,0);
    QPointF pt = trans.inverted(&bok).map(curPointF);
    pointLocation = pointInRect(pt);
    HIconGraphicsItem::mousePressEvent(event);
}

void HIconLineItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pt = event->scenePos() - curPointF;
    curPointF = event->scenePos();
    bool bok;
    QTransform trans;
    pLineObj->getTransform(trans,0);
    QPointF pt1 = trans.inverted(&bok).map(curPointF);
    if(pointLocation == 1)
    {
        QLineF lineF;
        lineF.setPoints(pt1,line().p2());
        setLine(lineF);
    }
    else if(pointLocation == 2)
    {
        QLineF lineF;
        lineF.setPoints(line().p1(),pt1);
        setLine(lineF);
    }
    else
    {
        //pLineObj->moveBy(pt.x(),pt.y());
        //QLineF lineF(pLineObj->getHeadPoint(),pLineObj->getTailPoint());
        QLineF lineF = line().translated(pt.x(),pt.y());
        setLine(lineF);
        if (flags() & ItemIsSelectable)
            setSelected(true);
    }
}

void HIconLineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    HIconGraphicsItem::mouseReleaseEvent(event);
}

void HIconLineItem::keyPressEvent(QKeyEvent *event)
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
    QPointF p1 = QPointF(line().p1().x() + ndx,line().p1().y() + ndy);
    QPointF p2 = QPointF(line().p2().x() + ndx,line().p2().y() + ndy);
    QLineF newLine = QLineF(p1,p2);
    setLine(newLine);
}

QLineF HIconLineItem::line() const
{
    if(pLineObj)
        return pLineObj->getObjLine();
    return QLineF();
}

void HIconLineItem::setLine(const QLineF &line1)
{
    if(line1 == line()) return;
    prepareGeometryChange();
    refreshBaseObj(line1);
    update();
}

void HIconLineItem::setItemObj(HBaseObj *pObj)
{
    pLineObj = (HLine*)pObj;
    pLineObj->setIconGraphicsItem(this);
}

HBaseObj* HIconLineItem::getItemObj()
{
    if(pLineObj)
        return pLineObj;
    return NULL;
}

void HIconLineItem::moveItemBy(qreal dx, qreal dy)
{
    QLineF newLineF;
    newLineF = line().translated(dx,dy);
    setLine(newLineF);
}

void HIconLineItem::refreshBaseObj(const QLineF& lineF)
{
    pLineObj->setHeadPoint(lineF.p1());
    pLineObj->setTailPoint(lineF.p2());
    pLineObj->setOX((lineF.p1().x() + lineF.p2().x())/2);
    pLineObj->setOY((lineF.p1().y() + lineF.p2().y())/2);
    pLineObj->setModify(true);
}

void HIconLineItem::resizeItem(const QPolygonF& polygonF)
{
    if(polygonF.size() != 2)
        return;
    QLineF newLineF(polygonF.at(0),polygonF.at(1));
    setLine(newLineF);
}

ushort HIconLineItem::pointInRect(QPointF &point)
{
    qreal halfpw = 14.00;
    QRectF rect1,rect2;
    rect1.setSize(QSizeF(halfpw,halfpw));
    rect1.moveCenter(line().p1());
    rect2.setSize(QSizeF(halfpw,halfpw));
    rect2.moveCenter(line().p2());
    ushort location = 0;
    if(rect1.contains(point))
        location = LOCATIONLEFT;
    else if(rect2.contains(point))
        location =  LOCATIONRIGHT;
    return location;
}

void HIconLineItem::setItemCursor(int position)
{
    if(position == 1)
        setCursor(QCursor(Qt::SizeFDiagCursor));
    else if(position == 2)
        setCursor(QCursor(Qt::SizeFDiagCursor));
    else
        setCursor(QCursor(Qt::ArrowCursor));
}

QRectF HIconLineItem::rect() const
{
    QPainterPath p;
    p.moveTo(line().p1());
    p.lineTo(line().p2());
    return p.boundingRect();
}

