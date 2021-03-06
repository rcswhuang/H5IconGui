#include "hiconpolygonitem.h"
#include "hpolygon.h"
#include <math.h>
//#include <QObject>
#include <QRectF>
#include <QPainterPath>
#include <QPointF>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QStyleOptionGraphicsItem>
#include <QPainter>

HIconPolygonItem::HIconPolygonItem(HIconGraphicsItem *parent)
    :HIconGraphicsItem(parent)
{

}

HIconPolygonItem::HIconPolygonItem(HBaseObj* obj, HIconGraphicsItem *parent)
    :HIconGraphicsItem(parent),pPolygonObj((HPolygon*)obj)
{
    pPolygonObj = NULL;
}

HIconPolygonItem::~HIconPolygonItem()
{
    /*
    if(pPolygonObj)
    {
        delete pPolygonObj;
        pPolygonObj = NULL;
    }*/
}

QRectF HIconPolygonItem::boundingRect() const
{
    return shape().controlPointRect();
}

bool HIconPolygonItem::contains(const QPointF &point) const
{
    return shape().boundingRect().contains(point);
}

void HIconPolygonItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QTransform transform;
    pPolygonObj->getTransform(transform,0);
    painter->setTransform(transform,true);
    pPolygonObj->paint(painter);
}

QPainterPath HIconPolygonItem::shape() const
{
    return pPolygonObj->shape();
}

int HIconPolygonItem::type() const
{
    return enumPolygon;
}

void HIconPolygonItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
   // HPropertyDlg dlg(pRectObj);
   // dlg.exec();
}

void HIconPolygonItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pointStart = event->scenePos();
    bool bok;
    QTransform trans;
    pPolygonObj->getTransform(trans,0);
    QPointF pt = trans.inverted(&bok).map(pointStart);
    pointLocation = pointInRect(pt);
    HIconGraphicsItem::mousePressEvent(event);
}

void HIconPolygonItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pt = event->scenePos() - pointStart;
    bool bok;
    QTransform trans;
    pPolygonObj->getTransform(trans,0);
    QPointF pt1 = trans.inverted(&bok).map(event->scenePos());
    pointStart = event->scenePos();

    if(pointLocation != 0)
    {
        int nRect = polygon().size();
        if(pointLocation > nRect)
            return;
        QPolygonF newPolygonF = polygon();
        if(pointLocation==1 || pointLocation==nRect)
        {
            newPolygonF.replace(0,pt1);
            newPolygonF.replace(nRect-1,pt1);
        }
        else
            newPolygonF.replace(pointLocation-1,pt1);
        setPolygon(newPolygonF);
    }
    else
    {
        //pPolygonObj->moveBy(pt.x(),pt.y());
        //QPolygonF polygonF = pPolygonObj->pylist;
        QPolygonF polygonF = polygon().translated(pt.x(),pt.y());
        setPolygon(polygonF);
        if (flags() & ItemIsSelectable)
            setSelected(true);
        //HIconGraphicsItem::mouseMoveEvent(event);
    }
}


void HIconPolygonItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    HIconGraphicsItem::mouseReleaseEvent(event);
}

void HIconPolygonItem::keyPressEvent(QKeyEvent *event)
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
    moveItemBy(ndx,ndy);
}

ushort HIconPolygonItem::pointInRect(QPointF& point)
{
    qreal halfpw = 6.00;
    int nRect = polygon().size();
    QRectF *pRect = new QRectF[nRect];
    for(int i = 0 ; i < nRect; i++)
    {
        pRect[i].setSize(QSizeF(halfpw,halfpw));
        pRect[i].moveCenter(polygon().at(i));
    }

    ushort location = 0;
    for(int j = 0;j < nRect;j++)
    {
        if(pRect[j].contains(point))
        {
            location = j+1;
            break;
        }
    }

    if(pRect)
    {
        delete[] pRect;
        pRect = NULL;
    }
    return location;
}

void HIconPolygonItem::setItemCursor(int location)
{
    if(location != 0)
        setCursor(QCursor(Qt::SizeFDiagCursor));
    else
        setCursor(QCursor(Qt::ArrowCursor));
}

void HIconPolygonItem::setPolygon(const QPolygonF & polygon1)
{
    if(polygon1 == polygon()) return;
    prepareGeometryChange();
    refreshBaseObj(polygon1);
    update();
}

QPolygonF HIconPolygonItem::polygon() const
{
    if(pPolygonObj)
        return pPolygonObj->pylist;
    return QPolygonF();
}

void HIconPolygonItem::refreshBaseObj(const QPolygonF& poly)
{
    pPolygonObj->pylist.clear();
    pPolygonObj->pylist = poly;
    //foreach(QPointF pt,pyVector)
    //    pPolygonObj->pylist.append(mapToScene(pt));
    QPointF p = mapToScene(polygon().boundingRect().center());
    pPolygonObj->setOX(p.x());
    pPolygonObj->setOY(p.y());
    pPolygonObj->width = polygon().boundingRect().width();
    pPolygonObj->height = polygon().boundingRect().height();
    pPolygonObj->setModify(true);
}

void HIconPolygonItem::setItemObj(HBaseObj* pObj)
{
    pPolygonObj = (HPolygon*)pObj;
    pPolygonObj->setIconGraphicsItem(this);
    //if(pPolygonObj)
   //     setPolygon(pyVector);
}

HBaseObj* HIconPolygonItem::getItemObj()
{
    if(pPolygonObj)
        return pPolygonObj;
    return NULL;
}

void HIconPolygonItem::moveItemBy(qreal dx,qreal dy)
{
    QPolygonF newPolygonF;// = polygon();
    foreach(QPointF pt,polygon())
    {
        pt.setX(pt.x()+dx);
        pt.setY(pt.y()+dy);
        newPolygonF<<pt;
    }
    setPolygon(newPolygonF);
}

void HIconPolygonItem::resizeItem(const QPolygonF& polygonF)
{
    if(polygonF.size() > polygon().size())
        return;
    setPolygon(polygonF);
}

//对于polyline polygon的setRect就是move object
void HIconPolygonItem::setRect(const QRectF& newRect)
{
    QRectF oldRect = rect();
    if(oldRect == newRect)
        return;
    prepareGeometryChange();
    qreal dx = newRect.center().x() - oldRect.center().x();
    qreal dy = newRect.center().y() - oldRect.center().y();
    moveItemBy(dx,dy);
    //update();
}

QRectF HIconPolygonItem::rect() const
{
    return polygon().boundingRect();
}
