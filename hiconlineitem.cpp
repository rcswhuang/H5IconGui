#include "H5IconGui/hiconlineitem.h"
#include "H5IconGui/hiconobj.h"
#include <math.h>
#include <QObject>
#include <QRectF>
#include <QPainterPath>
#include <QPointF>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QStyleOptionGraphicsItem>
#include <QPainter>


HIconLineItem::HIconLineItem(HIconGraphicsItem *parent):HIconGraphicsItem(parent)
{
    setFlags(QGraphicsItem::ItemIsMovable | QGraphicsItem::ItemIsSelectable);
    lineF.setLine(0,0,0,0);
}

HIconLineItem::HIconLineItem(const QLineF &line, HIconGraphicsItem *parent):HIconGraphicsItem(parent),lineF(line)
{
    //setAcceptDrops(true);
    pointLocation = LOCATIONNO;
    setFlag(QGraphicsItem::ItemIsMovable,true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);
    setFlag(QGraphicsItem::ItemIsFocusable,true);
    pLineObj = NULL;
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
    if(event->button() != Qt::LeftButton)
        return;
    curPointF = event->scenePos();
    if((pointLocation = pointInRect(curPointF))!=LOCATIONNO)
        lineMode = LineSize;

    HIconGraphicsItem::mousePressEvent(event);
}

void HIconLineItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pt = event->scenePos() - curPointF;
    curPointF = event->scenePos();
    if(lineMode == LineSize)
    {
        QLineF lineF;
        if(pointLocation == LOCATIONLEFT)
        {
           lineF.setPoints(line().p1()+pt,line().p2());
        }
        else if(pointLocation == LOCATIONRIGHT)
        {
            lineF.setPoints(line().p1(),line().p2()+pt);
        }
        setLine(lineF);
    }
    else
    {
        pLineObj->moveBy(pt.x(),pt.y());
        QLineF lineF(pLineObj->pfHeadPoint,pLineObj->pfTailPoint);
        setLine(lineF);
        //HIconGraphicsItem::mouseMoveEvent(event);
    }
}

void HIconLineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    lineMode = LineNo;
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
    return lineF;
}

void HIconLineItem::setLine(const QLineF &line)
{
    if(lineF == line) return;
    prepareGeometryChange();
    //lineF = line;
    //refreshBaseObj();
    pLineObj->pfHeadPoint = line.p1();
    pLineObj->pfTailPoint = line.p2();
    lineF = line;
    update();
}

void HIconLineItem::setItemObj(HBaseObj *pObj)
{
    pLineObj = (HLineObj*)pObj;
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

void HIconLineItem::refreshBaseObj()
{
    pLineObj->pfHeadPoint = mapToScene(line().p1());
    pLineObj->pfTailPoint =  mapToScene(line().p2());
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
    QPointF p1 = mapToScene(line().p1());
    QPointF p2 = mapToScene(line().p2());
    QRectF rectF1;
    rectF1.setSize(QSizeF(10,10));
    rectF1.moveCenter(p1);
    QRectF rectF2;
    rectF2.setSize(QSize(10,10));
    rectF2.moveCenter(p2);
    if(rectF1.contains(point))
        return LOCATIONLEFT;
    else if(rectF2.contains(point))
        return LOCATIONRIGHT;
    return LOCATIONNO;
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
