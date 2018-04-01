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

HIconPolygonItem::HIconPolygonItem(const QPolygonF &polygonF, HIconGraphicsItem *parent)
    :HIconGraphicsItem(parent),pyVector(polygonF)
{
    setFlag(QGraphicsItem::ItemIsMovable,true);
    setFlag(QGraphicsItem::ItemIsSelectable,true);
    setFlag(QGraphicsItem::ItemSendsGeometryChanges,true);
    setFlag(QGraphicsItem::ItemIsFocusable,true);
    pPolygonObj = NULL;
}

HIconPolygonItem::~HIconPolygonItem()
{
    if(pPolygonObj)
    {
        delete pPolygonObj;
        pPolygonObj = NULL;
    }
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
    pointLocation = pointInRect(pointStart);
    HIconGraphicsItem::mousePressEvent(event);
}

void HIconPolygonItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    qreal fRotateAngle = pPolygonObj->getRotateAngle();
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
        if(pointLocation==1 || pointLocation==nRect)
        {
            newPolygonF.replace(0,pointEnd);
            newPolygonF.replace(nRect-1,pointEnd);
        }
        else
            newPolygonF.replace(pointLocation-1,pointEnd);
        setPolygon(newPolygonF);
    }
    else
    {
        pPolygonObj->moveBy(pt.x(),pt.y());
        QPolygonF polygonF = pPolygonObj->pylist;
        setPolygon(polygonF);
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

void HIconPolygonItem::setItemCursor(int location)
{
    if(location != 0)
        setCursor(QCursor(Qt::SizeFDiagCursor));
    else
        setCursor(QCursor(Qt::ArrowCursor));
}

void HIconPolygonItem::setPolygon(const QPolygonF & polygon)
{
    if(pyVector == polygon) return;
    prepareGeometryChange();
    pyVector = polygon;
    refreshBaseObj();
    update();
}

QPolygonF HIconPolygonItem::polygon() const
{
    return pyVector;
}

void HIconPolygonItem::refreshBaseObj()
{
    pPolygonObj->pylist.clear();
    foreach(QPointF pt,pyVector)
        pPolygonObj->pylist.append(mapToScene(pt));
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
    if(pPolygonObj)
        setPolygon(pyVector);
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

QRectF HIconPolygonItem::rect() const
{
    return boundingRect();
}
