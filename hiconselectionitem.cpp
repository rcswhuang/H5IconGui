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
HIconSelectionItem::HIconSelectionItem(HIconGraphicsItem *parent)
    :HIconGraphicsItem(parent)
{

}

HIconSelectionItem::HIconSelectionItem(const QRectF &rectF, HIconGraphicsItem *parent)
    :HIconGraphicsItem(parent),rectF(rectF)
{
}

QRectF HIconSelectionItem::boundingRect() const
{
    //return shape().boundingRect();
    qreal pw = 20;
    return QRectF(rect().x() - pw/2,rect().y() - pw/2,rect().width() + pw,rect().height()+pw);
}

void HIconSelectionItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    painter->save();
    QPen pen = QPen(QColor(Qt::green));
    pen.setWidth(1);
    painter->setPen(pen);
    painter->drawRect(rect());
    painter->restore();
}

QPainterPath HIconSelectionItem::shape() const
{
    QPainterPath path;// = QGraphicsLineItem::shape();
    QRectF rectPath;
    rectPath.setX(rect().x() - 10);
    rectPath.setY(rect().y() - 10);
    rectPath.setWidth(rect().width() + 20);
    rectPath.setHeight(rect().height() + 20);
    path.addRect(rectPath);
    return path;

}

int HIconSelectionItem::type() const
{
    return enumSelection;
}

/*
void HIconSelectionItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    pointStart = event->scenePos();
    HIconGraphicsItem::mousePressEvent(event);
}

void HIconSelectionItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pt = event->scenePos() - pointStart;

    pointStart = event->scenePos();


        HIconGraphicsItem::mouseMoveEvent(event);

}


void HIconSelectionItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

    HIconGraphicsItem::mouseReleaseEvent(event);
}

*/
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
