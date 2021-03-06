#include "hiconpolylineitem.h"
#include "hpolyline.h"
#include <math.h>
//#include <QObject>
#include <QRectF>
#include <QPainterPath>
#include <QPointF>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QStyleOptionGraphicsItem>
#include <QPainter>

HIconPolylineItem::HIconPolylineItem(HIconGraphicsItem *parent)
    :HIconGraphicsItem(parent)
{

}

HIconPolylineItem::HIconPolylineItem(HBaseObj* obj, HIconGraphicsItem *parent)
    :HIconGraphicsItem(parent),pPolylineObj((HPolyline*)obj)
{
}

HIconPolylineItem::~HIconPolylineItem()
{
    /*
    if(pPolylineObj)
    {
        delete pPolylineObj;
        pPolylineObj = NULL;
    }*/
}

QRectF HIconPolylineItem::boundingRect() const
{
    return shape().controlPointRect();
}

bool HIconPolylineItem::contains(const QPointF &point) const
{
    return shape().boundingRect().contains(point);
}

void HIconPolylineItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    pPolylineObj->paint(painter);
}

QPainterPath HIconPolylineItem::shape() const
{
    return pPolylineObj->shape();
}

int HIconPolylineItem::type() const
{
    return enumPolyline;
}

void HIconPolylineItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
   // HPropertyDlg dlg(pRectObj);
   // dlg.exec();
}

void HIconPolylineItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pointStart = event->scenePos();
    pointLocation = pointInRect(pointStart);
    HIconGraphicsItem::mousePressEvent(event);
}

void HIconPolylineItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qreal fRotateAngle = pPolylineObj->getRotateAngle();
    QTransform transform;
    transform.rotate(-fRotateAngle);
    QPointF pointEnd = transform.map(event->scenePos());
    QPointF pt = pointEnd - transform.map(pointStart);
    transform.rotate(fRotateAngle);
    pointStart = event->scenePos();

    if(pointLocation != 0)
    {
        int nRect = polygon().size();
        if(pointLocation > nRect)
            return;
        QPolygonF newPolygonF = polygon();
        newPolygonF.replace(pointLocation-1,pointEnd);
        setPolygon(newPolygonF);
    }
    else
    {
        //pPolylineObj->moveBy(pt.x(),pt.y());
        //QPolygonF polygonF = pPolylineObj->pylist;
        QPolygonF polygonF = polygon().translated(pt.x(),pt.y());
        setPolygon(polygonF);
        if (flags() & ItemIsSelectable)
            setSelected(true);
    }
}


void HIconPolylineItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    HIconGraphicsItem::mouseReleaseEvent(event);
}

void HIconPolylineItem::keyPressEvent(QKeyEvent *event)
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

ushort HIconPolylineItem::pointInRect(QPointF& point)
{
    qreal halfpw = 14.00;
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

void HIconPolylineItem::setItemCursor(int location)
{
    if(location != 0)
        setCursor(QCursor(Qt::SizeFDiagCursor));
    else
        setCursor(QCursor(Qt::ArrowCursor));
}

void HIconPolylineItem::setPolygon(const QPolygonF & polygon1)
{
    if(polygon1 == polygon()) return;
    prepareGeometryChange();
    refreshBaseObj(polygon1);
    update();
}

QPolygonF HIconPolylineItem::polygon() const
{
    if(pPolylineObj)
        return pPolylineObj->pylist;
    return QPolygonF();
}

void HIconPolylineItem::refreshBaseObj(const QPolygonF& polygon1)
{
    pPolylineObj->pylist.clear();
    pPolylineObj->pylist = polygon1;
    QPointF p = mapToScene(polygon().boundingRect().center());
    pPolylineObj->setOX(p.x());
    pPolylineObj->setOY(p.y());
    pPolylineObj->width = polygon().boundingRect().width();
    pPolylineObj->height = polygon().boundingRect().height();
    pPolylineObj->setModify(true);
}

void HIconPolylineItem::setItemObj(HBaseObj* pObj)
{
    pPolylineObj = (HPolyline*)pObj;
    pPolylineObj->setIconGraphicsItem(this);
    //if(pPolylineObj)
    //    setPolygon(pyVector);
}

HBaseObj* HIconPolylineItem::getItemObj()
{
    if(pPolylineObj)
        return pPolylineObj;
    return NULL;
}

void HIconPolylineItem::moveItemBy(qreal dx,qreal dy)
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

void HIconPolylineItem::resizeItem(const QPolygonF& polygonF)
{
    if(polygonF.size() > polygon().size())
        return;
    setPolygon(polygonF);
}

//对于polyline polygon的setRect就是move object
void HIconPolylineItem::setRect(const QRectF& newRect)
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

QRectF HIconPolylineItem::rect() const
{
    return polygon().boundingRect();
}
