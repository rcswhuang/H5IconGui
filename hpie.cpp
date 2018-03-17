#include "hpie.h"
#include "hiconpieitem.h"
///////////////////////////////////////////////HPie//////////////////////////////////////
HPie::HPie()
{
    startAngle = 30;
    spanAngle = 240;
}

HPie::~HPie()
{

}

//二进制读写
void HPie::readData(QDataStream* data)
{
    if(!data) return;
    HRectangle::readData(data);
    int n;
    *data>>n;
    startAngle = n;
    *data>>n;
    spanAngle = n;
}

void HPie::writeData(QDataStream* data)
{
    if(!data) return;
    HRectangle::writeData(data);
    *data<<(int)startAngle;
    *data<<(int)spanAngle;
}

void HPie::readXml(QDomElement* dom)
{
    if(!dom) return;
    HRectangle::readXml(dom);
    startAngle = dom->attribute("startAngle").toInt();
    spanAngle = dom->attribute("spanAngle").toInt();
}

void HPie::writeXml(QDomElement* dom)
{
    if(!dom)return;
    HRectangle::writeXml(dom);
    dom->setAttribute("startAngle",startAngle);
    dom->setAttribute("spanAngle",spanAngle);
}

QString HPie::TagName()
{
    return "Pie";
}

//拷贝克隆
void HPie::copyTo(HBaseObj* obj)
{
    HPie* ob = (HPie*)obj;
    ob->topLeft = topLeft;
    ob->rectWidth = rectWidth;
    ob->rectHeight = rectHeight;
    ob->startAngle = startAngle;
    ob->spanAngle = spanAngle;
}

QRectF HPie::boundingRect() const
{
    return shape().controlPointRect();
}

bool HPie::contains(const QPointF &point) const
{
    return shape().contains(point);
}

QPainterPath HPie::shape() const
{
    QPainterPath path;
    if(nFillWay > 0 && nFillStyle > 0)
    {
        QRectF boundingRect = QRectF(topLeft,QSizeF(rectWidth,rectHeight)).adjusted(-5,-5,5,5);
        path.addRect(boundingRect);
        return path;
    }

    QRectF pathRect = QRectF(topLeft,QSizeF(rectWidth,rectHeight));
    QPolygonF polygon;
    polygon<<pathRect.topLeft()<<pathRect.topRight()<<pathRect.bottomRight()<<pathRect.bottomLeft();
    path.addPolygon(polygon);
    path.closeSubpath();
    QPainterPathStroker ps;
    ps.setWidth(10);
    return ps.createStroke(path);
}

DRAWSHAPE HPie::getShapeType()
{
    return enumPie;
}

void HPie::setStartAngle(int sAngle)
{
    startAngle = sAngle;
}

int HPie::getStartAngle()
{
    return startAngle;
}

void HPie::setSpanAngle(int spanangle)
{
    spanAngle = spanangle;
}

int HPie::getSpanAngle()
{
    return spanAngle;
}

void HPie::paint(QPainter* painter)
{
    HIconPieItem* pItem = qgraphicsitem_cast<HIconPieItem*>(getIconGraphicsItem());

    qreal fRotateAngle = getRotateAngle();
    QRectF rect(topLeft.x(),topLeft.y(),rectWidth,rectHeight);
    QPointF centerPoint = boundingRect().center();
    painter->save();
    if(pItem)
    {
        pItem->setTransformOriginPoint(centerPoint);
        QTransform transform;
        transform.rotate(fRotateAngle);
        pItem->setTransform(transform);
    }
    else
    {
        painter->rotate(fRotateAngle);
    }

    setPainter(painter,rect);//设置Painter

    int startAngle = getStartAngle()*16;
    int spanAngle = getSpanAngle()*16;
    painter->drawPie(rect,startAngle,spanAngle);

    if(pItem && pItem->isSelected())
    {
        painter->save();
        QPen pen1 = QPen(Qt::green);
        pen1.setWidth(1);
        painter->setPen(pen1);
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

        painter->drawRect(rect1);
        painter->drawRect(rect2);
        painter->drawRect(rect3);
        painter->drawRect(rect4);
    }
    painter->restore();
}
